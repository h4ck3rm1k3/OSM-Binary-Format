use strict;
use warnings;
use YAML;
sub LoadYML
{
    my $name =shift;
    open IN, "<${name}.yml";
    my $str;
    while(<IN>)
    {
        $str .= $_;
    }
    close IN;
    my $data= Load($str);
    return $data;
}

my @waycontents=@{LoadYML "waycontents"};

#warn Dump($waycontents[0])
#my ($hashref, $arrayref, $string) = Load(<<'...');
my $index=0;
my $size=scalar(@waycontents);
for my $x (@waycontents)
{
    print "int waynodes_${index} []= {" . join (",",@{$x},0) . "};\n";
    $index++;
}

$index=0;
print "int waycount = $size;";
print "int * waynodes[] = {";
for my $x (@waycontents)
{
    print "waynodes_${index},\n";
    $index++;
}
print "};\n";
