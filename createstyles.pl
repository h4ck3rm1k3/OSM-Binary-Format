
sub randcolor
{
    my @ca=('5','3','6','9','C','E');
    my @bgc=('','','');
    foreach my $c (@bgc){
        $r=int(rand(6));
        $c=$ca[$r].$ca[$r];
    }
    return "\#" . join("",@bgc);
}

for my $layer (0..39)
{
    my $lcolor= "#FFF000";
    open STYLE,">split/style_${layer}.xml";
    print STYLE qq[<?xml version="1.0" encoding="utf-8"?>
<Map bgcolor="transparent" srs="+proj=latlong +datum=WGS84">
	<FontSet name="book-fonts">
		<Font face_name="DejaVu Sans Book" />
	</FontSet>
      <Style name="layer_${layer}">
          <Rule>
            <PointSymbolizer 
allow_overlap="yes"
file="/home/mdupont/experiments/osm/mapnik/mapnik/symbols/station_small.png"
type="png"
width="3"
height="3"
/>
            <LineSymbolizer>
              <CssParameter name="stroke">${lcolor}</CssParameter>
            </LineSymbolizer>  
          </Rule>
        </Style>
	<Layer name="layer_${layer}" status="on" srs="+proj=latlong +datum=WGS84">
		<StyleName>layer_${layer}</StyleName>
		<Datasource>
			<Parameter name="type">osm</Parameter>
			<Parameter name="file">${layer}.osm</Parameter>
		</Datasource>
	</Layer>
</Map>
];
    close STYLE;
    system("python render.py $layer");

}
