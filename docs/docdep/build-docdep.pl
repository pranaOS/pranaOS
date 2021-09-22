use %include = ();
use %included = ();

for my $text (<*.txt>) {
    open I, '<', $text || die "cannot read: $text";
    while (<I>) {
	if (/^include::/) {
	    chomp;
	    s/^include::\s*//;
	    s/\[\]//;
	    $include{$text}{$_} = 1;
	    $included{$_} = 1;
	}
    }
    close I;
}