use Data::Dumper;
use strict;
use warnings;
my @nodeindex;
my %nodeids; # 
my @nodelat;
my @nodelon;
my $nodecount=0;
#use PDL;
=pod
    we want to be able to :
    break down the lat/lon into a set of differences between points so that we dont need to store the full position.
    we can store the position of one node only relative to the previous one for the drawing. the drawing can be done then with relative steps. We basically want to know if the object is closed or not. We have simple point, lines, boxes, polygon and then things like rivers, streets etc. 
    the idea is that we will want to avoid to have to store any extra way/relation information and that we can store all data in the right order. each point will know what the next point or points are in other relations/ways. there will be an index to show the first point for each way. 

    also we want to store some index for the nodes. Pages of them at different zoom levels, we want to split the nodes up like this.
    
=cut

sub addNode {
    my ($id,$lat, $lon)=@_;    
    my $nodeindex=$nodeids{$id};
    if (!$nodeindex){
        $nodeindex=$nodecount++;
        $nodeids{$id}=$nodeindex;
    };
    $nodelat[$nodeindex]=$lat;
    $nodeindex[$nodeindex]=$id;
    $nodelon[$nodeindex]=$lon;
}

my %wayids;
my @wayindex;
my $waycount=0;
sub addWay {
    my ($id)=@_;    
    my $wayindex=$wayids{$id};
    if (!$wayindex){
        $wayindex=$waycount++;
        $wayids{$id}=$wayindex;
    };
    $wayindex[$wayindex]=$id;
}

my @waycontents;
sub addNodeToCurrentWay
{
    my ($id)=@_;
    # watch the ordering of the nodes
    # 
    my $nodeindex=$nodeids{$id};
    if (!$nodeindex){
        die "Node $id not seend"
    };
    push @{$waycontents[$waycount-1]},$nodeindex; #lets create a simple array of nodes here
}


sub ProcessFile
{
    my $quote='[\'\"]';
    my $reid =qr[id=${quote}(-?\d+)${quote}\s*];
    my $rets =qr[(?:timestamp=${quote}\d+\-\d+\-\d+T\d+:\d+:\d+Z${quote})?\s*];
    my $reuid=qr[(?:uid=${quote}\d+${quote})?\s*];
    my $reusr=qr[(?:user=${quote}[^${quote}]+${quote})?\s*];
    my $remod=qr[(?:action=${quote}(?:modify|delete)${quote})?\s*];
    my $revis=qr[(?:visible=${quote}true${quote})?\s*];
    my $rever=qr[(?:version=${quote}\d+${quote})?\s*];
    my $recs =qr[(?:changeset=${quote}\d+${quote})?\s*];
    my $relat=qr[lat=${quote}(-?\d+\.\d+)${quote}\s*];
    my $relon=qr[lon=${quote}(-?\d+\.\d+)${quote}\s*];
    
    while(<>)
    {
        
        if (
         #   (/<node ${reid}${remod}${rets}${reuid}${reusr}${remod}${revis}${rever}${recs}${relat}${relon}/) ||
            (/<node ${reid}${relat}${relon}${rever}${recs}${reusr}${reuid}${rets}/)
            )
        {
            my $id=$1;
            my $lat=$2;
            my $lon=$3;
            addNode ($id,$lat,$lon);
        }
        elsif (/<node/)
        {
            die "what $_";
        }


        #<way id="30623039" version="13" changeset="10218143" uid="527441" user="dromedar" timestamp="2011-12-27T17:59:23Z"
        elsif (
            (/<way ${reid}${rever}${recs}${reusr}${reuid}${rets}/)
            )
        {
            my $id=$1;
            addWay ($id);
        }
        elsif (/<way/)
        {
            die "what $_";
        }
        elsif (/<nd ref="(\d+)"\/>/)
        {
            # 
            addNodeToCurrentWay ($1);
        }

        else
        {
            # skip
        }
        
    }
}

sub SortCrunch
{
    my @array=@_;
    my @sorted=sort @array;
    my @diff;
    my $start=$sorted[0];
    my $last=$start;
    push @diff,$start;
    my $factor=1000;
    for my $v (@sorted)
    {
#        push @diff,int(($last-$v) * $factor)/$factor; # round the data
        $v= int($v / 0.001);
#        push @diff, ;
        push @diff,$last-$v;
        $last=$v;
    }
    return @diff;
}

sub Unique
{
    my @array=@_;
    my %seen;
    map {$seen{$_}++; } @_;
    return %seen;
}

sub Report
{
    my %data=@_;
    for my $k (sort { $a <=> $b}keys %data) 
    {
        print $k . '=>' . $data{$k}. "\n";
    }
}

use YAML;
sub DumpYML
{
    my $name =shift;
    my $data =shift;
    open OUT, ">${name}.yml";
    print OUT Dump($data);
    close OUT;
}

#warn Dumper([SortCrunch @nodelat]);
#warn Dumper([SortCrunch @nodelon]);
#warn Dumper(\@nodelon);
#warn Dumper(\@nodeindex);

ProcessFile;

#warn Dumper({Unique SortCrunch @nodelat});
#Report Unique SortCrunch @nodelat;
#Report Unique SortCrunch @nodelon;
if (0)
{
    DumpYML "nodelat",\@nodelat;
    DumpYML "nodelon",\@nodelon;
    DumpYML "nodeindex",\@nodeindex;
    DumpYML "nodeids", \%nodeids;
    DumpYML "wayids",\%wayids;
    DumpYML "wayindex",\@wayindex;
}

DumpYML "waycontents", \@waycontents;
