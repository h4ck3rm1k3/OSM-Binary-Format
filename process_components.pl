#my %components;
my @components;

open IN,"components.txt";
while(<IN>)
{
    if (/(\d+)\t(\d+)/)
    {
#        my $nodeindex=$1;
        my $component=$2;
        push @components,$2;
#        $components{$nodeindex}=$component;
    }
}
close IN;

open(my $out, '>:raw', 'components.bin') or die "Unable to open: $!";
my $count=scalar(@components);
warn "Count is $count";
print $out pack ("L",$count);
print $out pack ("L*",@components);
close $out;
