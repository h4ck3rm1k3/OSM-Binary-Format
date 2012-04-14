use strict;
use warnings;
sub ProcessVector
{
    my $input=shift;
    my $output=shift;
    my @data;
    open IN,$input;
    while(<IN>)
    {
        if (/(\d+)/)
        {
            my $data=$1;
            push @data,$1;
        }
    }
    close IN;

    open(my $out, '>:raw', $output) or die "Unable to open: $!";
    my $count=scalar(@data);
    warn "Count is $count";
    print $out pack ("L",$count);
    print $out pack ("L*",@data);
    close $out;
}

ProcessVector("nodeindex.yml","nodeindex.bin");

