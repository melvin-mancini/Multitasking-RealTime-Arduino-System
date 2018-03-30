use strict; # make CPANTS happy
package CPAN::Reporter::API;
our $VERSION = '1.2006'; # VERSION

1;

# ABSTRACT: Programmer's interface to CPAN::Reporter



=pod

=head1 NAME

CPAN::Reporter::API - Programmer's interface to CPAN::Reporter

=head1 VERSION

version 1.2006

=head1 FUNCTIONS

CPAN::Reporter provides only a few public function for use within CPAN.pm.
They are not imported during C<<< use >>>.  Ordinary users will never need them.

=head2 C<<< configure() >>>

  CPAN::Reporter::configure();

Prompts the user to edit configuration settings stored in the CPAN::Reporter
C<<< config.ini >>> file.  It will create the configuration file if it does not exist.
It is automatically called by CPAN.pm when initializing the 'test_report'
option, e.g.:

  cpan> o conf init test_report

=head2 C<<< record_command() >>>

  ($output, $exit_value) = CPAN::Reporter::record_command( $cmd, $secs );

Takes a command to be executed via system(), but wraps and tees it to
show the output to the console, capture the output, and capture the
exit code.  Returns an array reference of output lines (merged STDOUT and
STDERR) and the return value from system().  Note that this is C<<< $? >>>, so the
actual exit value of the command will need to be extracted as described in
L<perlvar>.

If the command matches E<sol>\b(?:MakefileE<verbar>Build)\.PL\bE<sol>, then L<Devel::Autoflush>
is added to C<<< PERL5OPT >>> to force autoflushing of user prompts.

If the command includes a pipe character ('E<verbar>'), only the part of the 
command prior to the pipe will be wrapped and teed.  The pipe will be
applied to the execution of the wrapper script.  This is essential to 
capture the exit value of the command and should be otherwise transparent.

If a non-zero C<<< $secs >>> argument is provided, the command will be run with a
timeout of C<<< $secs >>> (seconds) if the appropriate process management modules are
available.  On Win32, L<Win32::Job> must be available; on Unix, L<Proc::Killfam>
must be available.  Otherwise, code will fall-back to running without a
timeout.

Also, on Win32, the first space-separated element of the command must be
absolute, or else ".exe" will be appended and the PATH searched for a matching
command.

If the attempt to record fails, a warning will be issued and one or more of 
C<<< $output >>> or C<<< $exit_value >>> will be undefined.

=head2 C<<< grade_make() >>>

  CPAN::Reporter::grade_make( $dist, $command, \@output, $exit);

Given a CPAN::Distribution object, the system command used to build the
distribution (e.g. "make", "perl Build"), an array of lines of output from the
command and the exit value from the command, C<<< grade_make() >>> determines a grade
for this stage of distribution installation.  If the grade is "pass",
C<<< grade_make() >>> does B<not> send a CPAN Testers report for this stage and returns
true to signal that the build was successful.  Otherwise, a CPAN Testers report
is sent and C<<< grade_make() >>> returns false.

=head2 C<<< grade_PL() >>>

  CPAN::Reporter::grade_PL( $dist, $command, \@output, $exit);

Given a CPAN::Distribution object, the system command used to run Makefile.PL
or Build.PL (e.g. "perl Makefile.PL"), an array of lines of output from the
command and the exit value from the command, C<<< grade_PL() >>> determines a grade
for this stage of distribution installation.  If the grade is "pass",
C<<< grade_PL() >>> does B<not> send a CPAN Testers report for this stage and returns
true to signal that the Makefile.PL or Build.PL ran successfully.  Otherwise, a
CPAN Testers report is sent and C<<< grade_PL() >>> returns false.

=head2 C<<< grade_test() >>>

  CPAN::Reporter::grade_test( $dist, $command, \@output, $exit);

Given a CPAN::Distribution object, the system command used to run tests (e.g.
"make test"), an array of lines of output from testing and the exit value from
the system command, C<<< grade_test() >>> determines a grade for distribution tests.
A CPAN Testers report is then sent unless specified prerequisites were not
satisfied, in which case the report is discarded.  This function returns true
if the grade is "pass" or "unknown" and returns false, otherwise.

=head2 C<<< test() >>> -- DEPRECATED

  CPAN::Reporter::test( $cpan_dist, $system_command );

This function is maintained for backwards compatibility.  It effectively 
wraps the functionality of C<<< record_command() >>> and C<<< grade_test() >>> into
a single function call. It takes a CPAN::Distribution object and the system
command to run distribution tests.

=head1 SEE ALSO

=over

=item *

L<CPAN::Reporter>

=item *

L<CPAN::Reporter::Config>

=item *

L<CPAN::Reporter::FAQ>

=back

=head1 AUTHOR

David Golden <dagolden@cpan.org>

=head1 COPYRIGHT AND LICENSE

This software is Copyright (c) 2006 by David Golden.

This is free software, licensed under:

  The Apache License, Version 2.0, January 2004

=cut


__END__

