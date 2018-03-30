# 
# This file is part of CPAN-Testers-Report
# 
# This software is Copyright (c) 2010 by David Golden.
# 
# This is free software, licensed under:
# 
#   The Apache License, Version 2.0, January 2004
# 
use 5.006;
use strict;
use warnings;
package CPAN::Testers::Fact::TestOutput;
BEGIN {
  $CPAN::Testers::Fact::TestOutput::VERSION = '1.999001';
}
# ABSTRACT: output from configuration, build and test

use Carp ();

use Metabase::Fact::Hash 0.016;
our @ISA = qw/Metabase::Fact::Hash/;

sub optional_keys { qw/configure build test/ }
  
1;



=pod

=head1 NAME

CPAN::Testers::Fact::TestOutput - output from configuration, build and test

=head1 VERSION

version 1.999001

=head1 SYNOPSIS

  my $fact = CPAN::Testers::Fact::TestOutput->new(
    resource => 'cpan:///distfile/RJBS/CPAN-Metabase-Fact-0.001.tar.gz',
    content => {
      configure => $makefile_pl_output,
      build     => $make_output,
      test      => $make_test_output,
    },
  );

=head1 DESCRIPTION

A fact to store output from configuration build and test.  Not generally used
if LegacyReport is used.

=for Pod::Coverage optional_keys

=head1 USAGE

See L<Metabase::Fact>.

=head1 BUGS

Please report any bugs or feature using the CPAN Request Tracker.  
Bugs can be submitted through the web interface at 
L<http://rt.cpan.org/Dist/Display.html?Queue=CPAN-Testers-Report>

When submitting a bug or request, please include a test-file or a patch to an
existing test-file that illustrates the bug or desired feature.

=head1 AUTHOR

  David Golden <dagolden@cpan.org>

=head1 COPYRIGHT AND LICENSE

This software is Copyright (c) 2010 by David Golden.

This is free software, licensed under:

  The Apache License, Version 2.0, January 2004

=cut


__END__



