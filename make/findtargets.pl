#!/client/bin/perl -w

use strict;
# use diagnostics;

use POSIX ('getcwd');

my $FALSE = \0;
my $TRUE  = \1;

my %deps         = ();
my %dirsByTarget = ();

{
#   print "\n============================================================\n";
#   print "============================================================\n\n";
    
    my $NameTags   = shift || &usage (0);
    my $LibTags    = shift || &usage (1);
    my @Makefiles  = @ARGV;
    
    my $dir        = undef;
    my @NameTags   = split ('\s*\|\s*', $NameTags);
    my @LibTags    = split ('\s*\|\s*', $LibTags);
    
    
    &usage unless (@Makefiles);
    
    foreach my $file (@Makefiles) {
	
#	generate dirname from filename
	if ($file =~ /^(.*)\/([^\\]*)$/) {
	    $dir = $1;
	} else {
	    $dir = getcwd ();
	}
	
     	&parse ($dir, $file, \@NameTags, \@LibTags);
    }
    
    &clean_foreign_deps  ();
#   &show_deps           ();
    &make_dirs_by_target ();
    &build_tree          ();
    &show_deps           ();
    &show_dirs           ();

    exit;
}



######################################################
#
# parse ($dir, $file, @NameTags, @LibTags)
#
# parses given MakeFile for used libs
#

sub parse ($$\@\@) {
    
    my $dir      = shift;
    my $file     = shift;
    my @NameTags = @{shift ()};
    my @LibTags  = @{shift ()};

    my $line     = undef;
    my $NameTag  = undef;
    my $LibTag   = undef;
    my %tmp_hash = ();
    my @targets  = ();
    my @liblines = ();
    
    my $appendTarget = $$FALSE;
    my $appendLib    = $$FALSE;
    
    open (IN, "<$file") || die "Cannot open $file!\n";
#   print "check $file for $NameTag and $LibTag\n";
    
    while (<IN>) {
	
#       get line w/o EOL
	$line = $_;
	chomp ($line);
	
#	throw away comments...
	$line = ( split (/#/, $line) )[0] || "";

#################################
#	package definition?	
	foreach my $tag (@NameTags) {
	    if ($line =~ /^\s*$tag\s*\+?=\s*([^\$]*?)\s*(\\)?\s*$/ ) {
		
#		print "   found: $1 ($line) with $tag\n";
		push (@targets, $1);
		$appendTarget = $2 ? $$TRUE : $$FALSE;
	    }
	}
#################################
#	cont. package definition?	
	if ($appendTarget) {
	    if ($line =~ /^\s*([^\$]*?)\s*(\\)?\s*$/o) {
		print "   Found: $1\n";
		push (@targets, $1);
	    }
	    $appendTarget = $2 ? $$TRUE : $$FALSE;
	}
	
#################################
#	lib definition?	
#       we use all tage _beginning_ with libTag! :-)
	foreach my $tag (@LibTags) {
	    if ($line =~ /^\s*$tag(?:.*?)=\s*(.*?)\s*(\\)?\s*$/) {
		
#	    	print "      FOUND: $1\n";
		push (@liblines, $1);
		$appendLib = $2 ? $$TRUE : $$FALSE;
	    } 
	}
	
#################################
#	cont. lib definition?
	if ($appendLib) {
    
	    if ($line =~ /^\s*(.+?)\s*(\\)?\s*$/o) {
#		print "      FOUND: $1\n";
		push (@liblines, $1);
	    }
	    $appendLib = $2 ? $$TRUE : $$FALSE;

	}
	
    }
    
#   don't need file anymore...
    close (IN);
    
#   ok, have all liblines for this package.. - which package btw?
    unless (scalar (@targets)) {return};
    
#   aaah.. - ok, so handle libs...
    my @tmp_libs = split (/ /, join  (" ", @liblines));

    my $target   = undef;
    my $lib      = undef;
    my @libs     = ( );
   
#   remeber all libs of form '-l<libname>'...
    foreach $lib (@tmp_libs) {

#	print "got lib: $lib\n";
	if ($lib =~ /^\s*-l(.*?)\s*$/o) {
	    
	    push (@{$deps{$dir}{'Libs'}}, $1);

	} elsif ($lib =~ /^\s*\$\((.*?)_LIB\)\s*$/) {

	    push (@{$deps{$dir}{'Libs'}}, $1);

	}
    }

#   we want to have unique targets!
    foreach $target (@targets) {
	$tmp_hash{$target}++;
    }
    
    @targets = keys (%tmp_hash);
    
#   and remember all targets made in this dir...
    foreach $target (@targets) {
	push (@{$deps{$dir}{'Targets'}}, $target);
    }
    
    return;
}



######################################################
#
# clean_foreign_deps ()
#

sub clean_foreign_deps ( )
{
    my $target      = undef;
    my $lib         = undef;
    my $dir         = undef;
    my @all_targets = ();
    
#   collect _all_ targets
    foreach $dir (keys (%deps)) {
	push ( @all_targets, @{$deps{$dir}{'Targets'}});
	$deps{$dir}{'Number'} = 1;
    }
    
    
    
    print "alltargets: '@all_targets'\n";
    
    foreach $dir (keys (%deps)) {
	
	@{$deps{$dir}{'DepLibs'}} = ();
	
	foreach $lib (@{$deps{$dir}{'Libs'}}) {
	
	    $lib = quotemeta ($lib);
	    push (@{$deps{$dir}{'DepLibs'}}, grep (/^$lib$/, @all_targets));
	}
    }
    
    return;
}




######################################################
#
# make_dirs_by_target ()
#

sub make_dirs_by_target ( )
{
    my $target = undef;
    my $dir    = undef;
    
    foreach $dir (sort (keys (%deps))) {
	foreach $target (@{$deps{$dir}{'Targets'}}) {
	    $dirsByTarget{$target} = $dir;
	}
    }
    
    return;
}




######################################################
#
# build_tree ()
#

sub build_tree ( )
{
    my $lib   = undef;
    my $dir   = undef;
    my $again = $$TRUE;
    my $count = 0;
    
    while ($again == $$TRUE) {
	
	$again = $$FALSE;
	
	foreach $dir (keys (%deps)) {
	
	    foreach $lib (@{$deps{$dir}{'DepLibs'}}) {
	
#		print "$dir: $lib: if ( $deps{$dirsByTarget{$lib}}{'Number'} " . 
#				 " <= $deps{$dir}{'Number'} )\n";
	
		if (   $deps{$dirsByTarget{$lib}}{'Number'}
		    >= $deps{$dir}{'Number'}               ) {
		    
		    $deps{$dir}{'Number'} = $deps{$dirsByTarget{$lib}}{'Number'} + 1;
		    $again = $$TRUE;
#		    print "YES. set $deps{$dir}{'Number'} \n";
		    
		}
	    }
	}
	
	$count++;
#	print "\n\n\nagain: $again\n\n";
	
	if ($count > 1000) {
	    
#	    die "Oops - this very much looks like recusive dependencies! Check by hand!";
	    warn "Oops - this very much looks like recusive dependencies! Check by hand!";
	    $again = $$FALSE;
	    
	}
    }
    
    return;
}




######################################################
#
# show_deps ()
#

sub show_deps ( )
{
    my $target = undef;
    my $dir    = undef;
    
    printf "%-60s:-----:-%-18s:-%-20s\n"       , '-'x60, '-'x18, '-'x30;
    printf "%-60s:     : %-18s: %-20s\n"       , ' 'x60, ' 'x18, ' 'x30;
    printf "%-60s: %3s : %-18s: depending on\n", 'directory', 'num', 'target' ;
    printf "%-60s:     : %-18s: %-20s\n"       , ' 'x60, ' 'x18, ' 'x30;
    printf "%-60s:-----:-%-18s:-%-20s\n"       , '-'x60, '-'x18, '-'x30;
    printf "%-60s:     : %-18s: %-20s\n"       , ' 'x60, ' 'x18, ' 'x30;
    
    foreach $dir (sort {$deps{$a}{'Number'} <=> $deps{$b}{'Number'} ||
		  	      $a            cmp       $b            } 
			      
			      (keys (%deps))) {
	
	foreach $target (sort (@{$deps{$dir}{'Targets'}})) {
	    printf "%-60s: %3i : %-18s: @{$deps{$dir}{'DepLibs'}}\n", 
		    $dir, $deps{$dir}{'Number'}, $target;
	}
    }
    printf ("%-60s:-----:-%-18s:-%-20s\n\n"     , '-'x60, '-'x18, '-'x30);

    return;
}




######################################################
#
# show_dirs ()
#

sub show_dirs ( )
{
    my $dir    = undef;
    
    foreach $dir (sort {$deps{$a}{'Number'} <=> $deps{$b}{'Number'} ||
		  	      $a            cmp       $b            } 
			      
			      (keys (%deps))) {
	
	    print "$dir ";
    }
    print "\n";

    return;
}




######################################################
#
# usage ()
#

sub usage ( )
{
    print STDERR "\nUSAGE: findtargets.pl " .
	         "<varname1> <varname2> "   . 
	     	 "<Makefile1> [<Makefile2> ...]\n\n";
    
    exit (1);
}

