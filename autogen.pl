#!/usr/bin/env perl

use Cwd;

my $dir = getcwd;

if (!-d "autoconf") {
    print "Creating autoconf directory\n";
    my $cmd = "mkdir -p autoconf/m4";
    system($cmd);
}

if (!-d "autoconf/m4") {
    print "Creating autoconf/m4 directory.\n";
    my $cmd = "mkdir autoconf/m4";
    system($cmd);
}

# Run autoreconf
print "==> Running autoreconf\n";
my $cmd = "autoreconf -ivf --warnings=all,no-obsolete,no-override -I autoconf";
system($cmd);
