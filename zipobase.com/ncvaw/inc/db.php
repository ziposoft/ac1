<?php


$obj_array=array();

function getobj($type) {
	global $root;
	global $obj_array;
	$objlist=null;

	if(!array_key_exists($type,$obj_array))
	{
		if (class_exists($type)) 
		{
			$filename=$root."/data/$type.pdata";
			if( file_exists ( $filename ))
			{
				$data = file_get_contents($filename);
				$objlist = unserialize($data);
			}
			else {
				$objlist=new $type();
				$objlist->create_from_spreadsheet();
				$data=serialize($objlist);
				file_put_contents($filename, $data);				
			}
			$obj_array[$type]=$objlist;
		}
		else {
			throw new Exception("Invalid product type given.");
		}
	}
	return $obj_array[$type];
}



function getj(&$row,$id)
{
	return $row->{	'gsx$'.$id }->{'$t' };
}

class data_source
{

	public $list;
	function create_from_spreadsheet()	
	{
	
	}

	function get_json_data($tab,$obj,$keyname=null,$spreadsheetid=null) 
	{
		global $root;
		global $refresh_data;
		
		$this->list=array();
		$cn=get_class($this);
		$filename=$root."/data/$cn.json";
		if(! file_exists ( $filename ))	
			$refresh_data=1;
		if($refresh_data)
		{
			if(!$spreadsheetid)
			{
				$spreadsheetid = '0AonA9tFgf4zjdHhNd1FIeFJzVWRrdDlUangxWUlkTXc';
			}
			//http://spreadsheets.google.com/feeds/list/xvzkey/1/public/values?alt=json
			//https://spreadsheets.google.com/feeds/list/0AonA9tFgf4zjdHhNd1FIeFJzVWRrdDlUangxWUlkTXc/1/public/values?alt=json
			$url = "http://spreadsheets.google.com/feeds/list/$spreadsheetid/$tab/public/values?alt=json";
			$file = file_get_contents ( $url );
			$fp = fopen ( $filename, 'w' );
			fwrite ( $fp, $file );
			fclose ( $fp );
		}
			
		$json = json_decode (  file_get_contents ( $filename ) );
		if($json)
			$jdata = $json->{'feed' }->{'entry' };	
		else 
		{
			throw new Exception("Could not get JSON data for $obj");
			return;
		}
		$first=1;
		foreach ($jdata as $row )
		{
			
			if($first)
			{
				$first=null;
				continue;
			}
			if($keyname)
			{
				$key =getj($row,$keyname);
				$this->list [$key] = new $obj ( $row );
			}
			else 
				$this->list [] = new $obj ( $row );
				
		}	
	}
}

class bill {
	public $nickname;
	public $stance;
	public $effect;
	public $official;
	public $svid;
	public $hvid;	
	public $desc;
	public $year;
	public $doc;
	public $picture;
	public function __construct($d)
	{
		
		$this->nickname =getj($d,'nickname');
		$this->stance =getj($d,'stance');
		if($this->stance == 'pro')
		{
			$this->effect= "Supports animal welfare";
			$this->picture='bill_good.png';
		}
		else 
		{
			$this->picture='bill_bad.gif';
			$this->effect= 	"Harmful to animal welfare";
		}
		$this->official =getj($d,'official');
		$this->svid =getj($d,'svid');
		$this->hvid =getj($d,'hvid');		
		$this->desc =getj($d,'desc');
		$this->year =getj($d,'year');		
		$this->doc =getj($d,'doc');		
	}
	public function get_stance()
	{
		if($this->stance == 'pro')
		{
			return "Supports animal welfare";
		}
		
		return 	"Harmful to animal welfare";
		
	
	}
	public function print_tr()
	{
		echo ("<div><h3><a href='/guide/billpage.php?doc=$this->doc'>$this->year - $this->doc - $this->nickname <img style='display:inline;width:40px' src='/img/$this->picture' /></a></h3>	
			
			<div>$this->official </div>
		<div>$this->effect </div>
		<div><a target='_blank' href='http://www.ncleg.net/gascripts/BillLookUp/BillLookUp.pl?Session=$this->year&BillID=$this->doc&submitButton=Go' >Link to $this->doc on NCLEG.NET</a> </div>
		<div>$this->desc </div></div>");
	}
	public function print_page()	
	{
		echo "<H1>$this->year - $this->doc -  $this->nickname</H1>";
		echo "<H2>$this->official </H2>";
		
		
		echo "<p><img style='display:inline;width:60px' src='/img/$this->picture' /> $this->effect </p>";		
		echo "<p><a target='_blank' href='http://www.ncleg.net/gascripts/BillLookUp/BillLookUp.pl?Session=$this->year&BillID=$this->doc&submitButton=Go' >Link to $this->doc on NCLEG.NET</a> </p>";
		echo "<p>$this->desc </p>";
	}	
}

class bill_list extends data_source
{
	
	function create_from_spreadsheet()
	{
		$this->get_json_data(1,'bill','doc');
	}	
	public function get_bill($doc)
	{
		return $this->list [$doc];
	}
	public function print_bills()
	{
		foreach ( $this->list as $bill )
		{
			$bill->print_tr();
		}
	}	
}

class vote {
	public $vid;
	public $mkey;
	public $doc;
	public $vote;
	public $score;
	public $grade; /* -1 bad vote, 0 neutral, 1 good */
	public function __construct($d)
	{
		$this->vid=getj($d,'vid');
		$this->mkey=getj($d,'key');		
		$this->doc= getj($d,'doc');
		$this->vote=getj($d,'vote');
		$this->grade=0;
	}
	public function get_score()
	{
		$bill=getobj("bill_list")->get_bill($this->doc);
		
		if($this->vid)
		{
			if(!(($this->vid==$bill->svid)||($this->vid==$bill->hvid)))
				return 0;
		}
		if(($this->vote=='Aye') xor($this->vote=='No'))
		{
			if(($this->vote=='Aye') xor ($bill->stance=='pro'))
				$this->grade=-1;
			else
				$this->grade=1;
			return $this->grade;
		}

		if(($this->vote=='psp') xor($this->vote=='sp'))
		{
			if($bill->stance=='anti')
				$this->grade=-1;
			else
				$this->grade=1;
			return $this->grade * 3;
		}
		return $this->grade;
	}

	public function print_vote_tr()
	{
		$vote=$this->vote;
		$doc=$this->doc;
		$bill=getobj("bill_list")->get_bill($doc);
		if($this->vid)
		{
			if(!(($vid==$bill->svid)||($vid==$bill->hvid)))
				return;
		}
		//echo "<img style='width:50px' src='/img/x.jpg'/>";

		echo "<tr>";
		//echo "<td>$vid</td>";

		$picture=0;
		$class="";
		$title="Voted ".($vote=='Aye'? "for" :"against")." bill that ". $bill->effect;
		if(($vote=='Aye') xor($vote=='No'))
		{
			if(($vote=='Aye') xor ($bill->stance=='pro'))
			{
				$class='votebad';
				$picture='bill_bad.gif';
			}
			else
			{
				$class='votegood';
				$picture='bill_good.png';
			}
		}

		if(($vote=='psp') xor($vote=='sp'))
		{
			$title='Bill ' . $bill->effect;
			if($bill->stance=='anti')
			{
				$class='votebad';
				$picture='bill_bad.gif';
			}
			else
			{
				$class='votegood';
				$picture='bill_good.png';
			}
		}
		if($vote=='psp')
			$vote='Primary Sponsor';
		if($vote=='sp')
			$vote='Sponsor';
		if($vote=='N/V')
			$vote='Did Not Vote';
		echo "<td><div class='$class'>$vote</div>";
		if($picture)
			echo "<img style='width:60px' title='$title' src='/img/$picture'/>";

		echo "</td>";
		echo "<td><div><a href='/guide/billpage.php?doc=$doc'>$doc - $bill->official</a></div>";
		echo "<div class='$class'>$title</div>";
		if($bill->desc)
			echo "<div>$bill->desc</div>";

		echo "</td></tr>";
	}
}

class vote_data extends data_source
{

	function create_from_spreadsheet()
	{
		$this->get_json_data(3,'vote');
	}	

	public function print_bill_votes($title,$doc,$vote,$vid) {

		$legs=array(); //local
		//echo "<table class='votes'>";
		$comma=0;
		$uid=$doc.$vote.$vid;
		$display='block';

		
		foreach ( $this->list as $v )
		{
			if($vid && ($vid!=$v->vid))
				continue;
			if(($doc == $v->doc)&&
					($vote==$v->vote)
			)
			{
				
				$leg=getobj("leg_list")->get_leg_by_key($v->mkey);
				if($leg)
					$legs[]=$leg;
			}
		}
		$count=count($legs);
		if($count>6)
			$display='none';
		echo("<h5>$title:<a onclick='togglehide(\"$uid\")'>($count)</a></h5><div id='$uid' style='display:$display'>");



		foreach ( $legs as $leg )
		{

			if($comma)
				echo (", ");
			echo "<a href='/guide/legpage.php?id=$leg->key'>$leg->name</a>";
			$comma=1;
		}
		echo("</div>");
	}
	public function get_votes($legid,&$count) {
		$score=0;
		$count=0;
        
		foreach ( $this->list as $v )
		{
			if($legid == $v->mkey)
			{

				$score+=$v->get_score();
				$count++;

			}
		}
		return $score;
	}
	public function print_list_votes($legid,$sponsors) {
		foreach ( $this->list as $v )
		{
			if($v->mkey xor $sponsors)
				if($legid == $v->mkey)
				{
					$v->print_vote_tr();
				}
		}
	}
}

function get_grade(&$score,&$grade,&$color)
{
	$grades = [
	 [ -3,"F","#F00"],
	 [ -2,"D-","#C04"],	 
	 [ -1,"D","#808"],
	 [ 0,"C","#00F"],
	 [ 1,"C+","#02E"],	 
	 [ 2,"B","#088"],	 	
	 [ 3,"B+","#0c8"],	  
	 [ 4,"A-","#0c0"],
	 [ 5,"A","#0c0"],	 
 	 [ 6,"A+","#0c0"],	 
	];	
	if(!$grade)
	{
		$grade="A+";
		$color="#0c0";
		foreach ( $grades as $g )
		{
			if($score <= $g[0])
			{
				$grade=$g[1];
				$color=$g[2];
				return;
			}
		}
		return;
	}
	foreach ( $grades as $g )
	{
		if($grade == $g[1])
		{
			$score=$g[0];
			$color=$g[2];
			return;
		}
	}	
}


class legislator{

	public $name;
	public $first;
	public $last;
	public $id;
	public $key;
	public $grade;
	public $score;
	public $comment;
	public $grade_color;
	public $uid;
	public $party;
	public $chamberId;
	public $chamber;
	public $county;
    public $url;
	public $district;

	public function __construct($d) {

		$this->first = getj($d,'first');
		$this->last = getj($d,'last');
		$this->key = getj($d,'key');
		$this->comment=getj($d,"comment");
		$this->grade=getj($d,"grade");

		$this->name = $this->first.' '.$this->last;
		$this->uid =getj($d,'uid');
		$this->id = getj($d,'id');
		$this->county = getj($d,'county');
		$this->chamberId = getj($d,'chamber');
        $this->district=getj($d,'district');
        $this->title=getj($d,'title') ;
		if($this->chamberId=='H')
			$this->chamber='House';
		else
			$this->chamber='Senate';
			
		$this->create_grade();
	}
	public function get_url() {
		return ("http://www.ncleg.net/gascripts/members/viewMember.pl?sChamber=$this->chamber&nUserID=$this->uid");
	}
	public function print_survey() {
		getobj("survey_data")->printresp($this->key);
	}
	public function print_list_votes()
	{
		getobj("vote_data")->print_list_votes($this->id,0);

	}
	public function print_list_sponsorship()
	{

		getobj("vote_data")->print_list_votes($this->id,1);

	}
	public function create_grade()
	{
		$count=0;
		$color="#000";

		$score=getobj("vote_data")->get_votes($this->key,$count);

		if( ($count==0) && (!$this->grade))
		{
			$this->grade='Not Graded';
			$this->comment='Has not been in office long enough to assign grade';
		}
		else
			get_grade($score,$this->grade,$color);


		$this->grade_color=$color;
		$this->score=$score;
			
	}

	public function print_table_row($label, $val,$color=null) {
		$style="";
		if($color)
			$style="style='color:$color;font-weight:bold'";
			
		echo "<tr><td class='leg_label'>$label: </td><td class='leg_val' $style>$val</td></tr>";
	}
	public function print_list_row() {
		global $isPhone;
		$canidate=getobj("canidates")->get_candiate($this->key);
		$data_key=$this->key;


		echo "<div class='leg_bio' data-name='$data_key'><hr/><div class='leg_thumb' >";
		echo "<a title='Click for voting record'  href='/guide/legpage.php?id=$this->key'>";


		echo "<img src='http://www.ncleg.net/$this->chamber/pictures/$this->uid.jpg'/></a>";
		
		echo "</div><div class='leg_info' ><a title='Click for voting record' href='/guide/legpage.php?id=$this->key'><h2>$this->title $this->name</h2></a><table>";

		/*
		 $district=
		*/
		
		$district_url="'/district.php?dist=". $this->district . "&ch=" . $this->chamberId . "'";
		$this->print_table_row ( 'District', "<a title='Show district race'  href=$district_url>$this->district</a>" );
		$running="Not running for re-election.";
		if($canidate)
		{
			$running=$canidate->get_running();

				
		}
		$this->print_table_row ( '2014 Election', $running );
		$this->print_table_row ( 'Party', $this->party );
		$this->print_table_row ( 'Counties', $this->county );

		$email_link="<a  href='mailto:'" . getj($d,'email') . ">" .  getj($d,'email') . "</a>";
		$this->print_table_row ( 'Email', $email_link );
		$this->print_table_val ( 'Phone', $this->phone );

		if(option('grades'))
		{
			$grade_link="<a title='Click for voting record'  style='color:" .$this->grade_color
			."' href='/guide/legpage.php?id=$this->key'>$this->grade</a>";
			$this->print_table_row ( 'Grade',$grade_link );
				
		}


		if(!$this->comment)
		{
				
			$this->comment="Voting record, responsiveness to inquiries, and feedback from constituents";
			$this->print_table_row ( 'Ranking Created By', $this->comment );
		}
		else
			$this->print_table_row ( 'Reason For Grade', $this->comment );





		echo '</table>';
        $url=$this->get_url();
		echo ("<a target='_blank' href='$url'>Link to page on NCGA website</a>&nbsp;&nbsp;&nbsp;&nbsp;<a  href='/guide/legpage.php?id=$this->key'>Link to voting record</a>");
		echo "</div></div><div style='clear:both'></div>";
	}
}



function sort_func_grade($a, $b) {
	if ($a->score == $b->score) {
		return 0;
	}
	return ($a->score < $b->score) ? 1 : -1;
}
function sort_func_dist($a, $b) {
	if ($a->distrcit == $b->distrcit) {
		return 0;
	}
	return ($a->distrcit < $b->distrcit) ? -1 : 1;
}

class leg_list extends data_source{
    function create_from_spreadsheet()
	{
		$this->get_json_data(2,'legislator','key');
	}
	public function print_list() {
		echo "<div class='tbl_leglist' >";
		foreach ( $this->list as $d ) {
			$d->print_list_row ();
		}
		echo '</div>';
	}
   
	public function get_leg_by_key($key) {
        if(array_key_exists ($key,$this->list))
            return $this->list[$key];
        return null;
	}
	
	public function get_leg_by_district($district,$chamber) {
		foreach ( $this->list as $leg ) {
			if (($district == $leg->district)
                &&($chamber==$leg->chamberId))
				return $leg;
		}
		return 0;
	}
	public function sort() {
		
		$sort=getParam("sort");
		if($sort=='grade')
		{
			uasort($this->list, 'sort_func_grade');
		}
		else
		if($sort=='grade')
		{
			uasort($this->list, 'sort_func_dist');
		}	
		else
			ksort ( $this->list );
	
	}	
	
}

class canidate {
	public $displayname;
	public $key;
	public $party;
	public $party_id;
	public $first;
	public $last;

	public $id;
	public $uid;
	public $chamberId;
	public $chamber;
	public $url_cover_jpg;

	public $jpg_path;
	public $url;
	public $photo;
	public $website;
	public $email;
	public $election;



	public function __construct($data_in) {
		$this->data = $data_in;
		$this->displayname=$this->get('nameonballot');
		$this->party_id = $this->get('party');
		$this->chamberId = $this->get('chamber');
		$this->key = $this->get('key');
		$this->party=$this->party_id;
		if($this->party_id=='DEM')
		{
			$this->party='Democratic';
		}
		if($this->party_id=='REP')
		{
			$this->party='Republican';
		}
		if($this->chamberId=='H')
			$this->chamber='House';
		else
			$this->chamber='Senate';
			
	}
	public function print_table_val($label, $field) {
		$val = $this->data->{	'gsx$' . $field }->{'$t' };
		if(!$val)
			return;
		$this->print_table_row ( $label, $val );
	}
	public function print_table_row($label, $val) {
		echo ("<tr><td class='leg_label'>$label: </td><td class='leg_val'>$val</td></tr>");
	}
	public function get_running() {
		$running="Running for re-election in the ";

		if($this->get('election')=='gen')
		{
			$running.='general election 11/4/2014';
				
		}
		else
		{
			$running.=$this->party . ' primary election 5/6/2014';
		}
		return $running;
	}
	public function print_list_row() {

		$leg=getobj("leg_list")->get_leg_by_key($this->key);
		if($leg)
		{
			$leg->print_list_row();
			return;
		}
		$data_key=$this->key;
		$lastname=strtolower($this->get('last'));
		
		echo ("<div class='leg_bio' data-name='$data_key'><hr>");
		//thumbnail
		$photo = $this->get('photo');
		if($photo)
		{

			echo ("<div class='leg_thumb' ><a href='/guide/canidate.php?key=$this->key'>");
			echo ("<img src='$photo'/></a></div>");
		}

		echo ("<div class='leg_info' ><a href='/guide/canidate.php?key=$this->key'><h2>$this->displayname</h2></a><table><tr><td/><td/></tr>");
		$district=$this->get('district');
		$district_url="'/district.php?dist=". $district . "&ch=" . $this->chamberId . "'";
		$this->print_table_row ( 'District', "<a href=$district_url>$district</a>" );

		$this->print_table_row ( 'Party', $this->party );
		$running="Challenger in the ";

		if($this->get('election')=='gen')
		{
			$running.='general election 11/4/2014';
				
		}
		else
		{
			$running.=$this->party . ' primary election 5/6/2014';
		}
		
		$this->print_table_row ( '2014 Election', $running );
		$website= $this->get('website');
		if($website)
		{
			$link="<a href='".$website."' target='_blank'>".$website."</a>";
			$this->print_table_row ( 'Webiste', $link );
				
		}
		
		
		$this->print_table_val ( 'Email', 'email' );
		$this->print_table_val ( 'Phone', 'phone' );

		echo ('</table>');
			
		echo ("</div></div><div style='clear:both'></div>");
	}

}


class canidates extends data_source
{
	public function __construct() {
		$this->get_data(5);
		$this->list=array();
		
	}	
	public function get_candiate($key)
	{
		foreach ( $this->rows as $row )
		{
			if($key==getj($row,'key'))
				return new canidate ( $row );
		}
		return null;
	}	
	public function getlist($ch,$num,$elect)
	{
		$set=array();
		foreach ( $this->rows as $row )
		{
			if( (getj($row,'district')==$num)
					 &&(getj($row,'chamber')==$ch)
					&&(getj($row,'election')==$elect))
			{
				$set [] = new canidate ( $row );
			}	
		}
		return $set;
	}
	
	public function printlist($ch,$num,$elect)
	{
		
	
		foreach ( $this->rows as $row )
		{
			
			if( (getj($row,'district')==$num)
					 &&(getj($row,'chamber')==$ch)
					&&(getj($row,'election')==$elect))
			{
					
					$x = new canidate ( $row );
					
					$x->print_list_row();
					
					
				}	
			
		}
		
	}	
}



class district {
	public function __construct($data_in) {
		$this->data = $data_in;
	}	

}

class districts extends data_source
{
	public function __construct() {
		$this->get_data(4);
		$this->list=array();
		
	}	
	
	public function printlist($ch)
	{
	
		
		return 0;
	}	
	public function get($ch,$num)
	{

		foreach ( $this->rows as $row )
		{
			if( (getj($row,'district')==$num) &&(getj($row,'chamber')==$ch))
			{
				
				return new district($row);
			}	
			
		}
		return 0;
	}
}
?>

