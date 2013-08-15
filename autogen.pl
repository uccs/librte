#!/usr/bin/env perl

# Run autoreconf
print "==> Running autoreconf\n";
my $cmd = "autoreconf -ivf --warnings=all,no-obsolete,no-override -I autoconf";
system($cmd);
