<?php


$obj_array=array();

function getobj($type) {
	global $obj_array;
	if(!array_key_exists($type,$obj_array))
	{
		if (class_exists($type)) {
			$obj_array[$type]= new $type();
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
class json_obj
{
	public $data;
	public function get($id)
	{
		return $this->data->{	'gsx$'.$id }->{'$t' };
	}
	
}
class data_source
{
	public $rows;

	function get_data($tab,$id=null) 
	{
		global $root;
		global $refresh_data;
		$this->rows=null;
		$cn=get_class($this);
		$filename=$root."/data/$cn.json";
		if(! file_exists ( $filename ))	
			$refresh_data=1;
		if($refresh_data)
		{
			if(!$id)
			{
				$id = '0AonA9tFgf4zjdHhNd1FIeFJzVWRrdDlUangxWUlkTXc';
				//$id = '1pjXSrS-VfXiRknB6ymTqxhzuokY39tmBRDITwf_h5DM';
				
				
			}
			//http://spreadsheets.google.com/feeds/list/xvzkey/1/public/values?alt=json
			//https://spreadsheets.google.com/feeds/list/0AonA9tFgf4zjdHhNd1FIeFJzVWRrdDlUangxWUlkTXc/1/public/values?alt=json
			$url = "http://spreadsheets.google.com/feeds/list/$id/$tab/public/values?alt=json";
			$file = file_get_contents ( $url );
			$fp = fopen ( $filename, 'w' );
			fwrite ( $fp, $file );
			fclose ( $fp );
		}
			
		$json = json_decode (  file_get_contents ( $filename ) );
		if($json)
			$this->rows = $json->{'feed' }->{'entry' };	
				
	}
}

class vote extends json_obj{
	public $vid;	
	public $doc;	
	public $vote;	
	public $score;	
	public $grade; /* -1 bad vote, 0 neutral, 1 good */	
	public function __construct($data_in)
	{
		$this->data = $data_in;
		$this->vid=$this->get('vid');
		$this->doc= $this->get('doc');
		$this->vote= $this->get('vote');
	}
	public function get_score()
	{
		$bill=getobj("bill_list")->get_bill($this->doc);	
		$this->grade=0;
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
	
		$vid=$this->get('vid');
		$doc= $this->get('doc');
		$vote= $this->get('vote');	

		$bill=getobj("bill_list")->get_bill($doc);
		if($vid)
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
		
		
			/*echo "</td>";

	
		$title="Voted ".($vote=='Aye'? "for" :"against")." bill that ". $bill->effect; 
	
			
		echo "<td><span class='$class'>$vote</span></td><td>";

		if($class=='votebad')
		{
			echo "<img title='$title' src='/img/x.png'/>";
			
		}
		if($class=='votegood')
		{
			echo "<img  title='$title' src='/img/check.png'/>";
				
		}		*/
		echo "</td></tr>";
	}
}
class bill extends json_obj{
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
	public function __construct($data_in)
	{
		$this->data = $data_in;
		$this->nickname =$this->get('nickname');
		$this->stance =$this->get('stance');
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
		$this->official =$this->get('official');
		$this->svid =$this->get('svid');
		$this->hvid =$this->get('hvid');		
		$this->desc =$this->get('desc');
		$this->year =$this->get('year');		
		$this->doc =$this->get('doc');		
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
class vote_data extends data_source
{
	public $list;
	public function __construct() {
		$this->get_data(3);

	}
	public function get_bill($vid)
	{

		$this->list=array();
		foreach ( $this->rows as $row )
		{
			$vid =$row->{	'gsx$vid' }->{'$t' };
			$this->list [$vid] = new bill ( $row );
		}
	}
	public function get_votes_by_mkey($legid) {
		$list=array();
		foreach ( $this->rows as $row )
		{
			if($legid ==  getj($row,'key'))
			{
				$list[getj($row,'vid')]=new vote($row);
			}
		}
		return $list;
	}	
	public function get_votes_by_mid($legid) {
		$list=array();
		foreach ( $this->rows as $row )
		{
			if($legid ==  getj($row,'mid'))
			{
				$list[getj($row,'vid')]=new vote($row);
			}
		}
		return $list;
	}	
	public function print_bill_votes($title,$doc,$vote,$vid) {
	
	
		//echo "<table class='votes'>";
		$comma=0;
		$uid=$doc.$vote.$vid;
		$display='block';
		
		$list=array();
		foreach ( $this->rows as $row )
		{
			if($vid && ($vid!=getj( $row,'vid')))
				continue;
			if(($doc ==getj( $row,'doc'))&&
				($vote==getj( $row,'vote'))
				)
			{
				$mid= $row->{	'gsx$key' }->{'$t' };
				$leg=getobj("leg_list")->get_leg_by_key($mid);
				if($leg)
					$list[]=$leg;
			}
		}
		$count=count($list);
		if($count>6)
			$display='none';
		echo("<h5>$title:<a onclick='togglehide(\"$uid\")'>($count)</a></h5><div id='$uid' style='display:$display'>");
		
		
		
		foreach ( $list as $leg )
		{
		
			if($comma)
				echo (", ");
			echo "<a href='/guide/legpage.php?id=$leg->key'>$leg->name</a>";
			$comma=1;
		}
		echo("</div>");
	}	
	public function get_votes($legid,&$list) {
		$score=0;
		foreach ( $this->rows as $row )
		{
			if($legid == getj($row,'key'))
			{
				$v=new vote($row);
				$list[]=$v;
				$score+=$v->get_score();
				
			}
		}
		return $score;
	}	
	public function print_list_votes($legid,$sponsors) {


		foreach ( $this->rows as $row )
		{

			if(getj($row,'vid') xor $sponsors)
				if($legid == getj($row,'mid'))
				{
					$vote=new vote($row);
					$vote->print_vote_tr();
				}
		}
	}	
}


class survey_questions extends data_source
{
	public function __construct() {
		$this->get_data(7);

	}
	public function getquestion($num)
	{

		return  getj($this->rows[$num-1],"question");
	}
}
class survey_data extends data_source
{
	public function __construct() {
		$this->get_data(1,"0AonA9tFgf4zjdE45M0MyZTR0UUYxXzNzRjBuNWFnMGc");

	}
	public function getrow_bykey($key)
	{
		foreach ( $this->rows as $row )
		{
			if (getj($row,'key')==$key)
			{
				return $row;
			}
		}
		return null;
	}	
	public function getrow($first,$last)
	{
		foreach ( $this->rows as $row )
		{
			$f=getj($row,'firstname');
			$l=getj($row,'lastname');
			if( ($l==$last) &&($f==$first))
			{
				return $row;
			}
		}
		return null;
	}	
	public function printresp($key)
	{
		$row = $this->getrow_bykey($key);
		if(!$row)
        {
            echo("<div>Did not respond to our survey.</div>");    
			return;
        }
		echo("<div  style='max-width:800px'><h3>Resposes to animal welfare survey:</h3>");    
		for ($x=1; $x<=5; $x++)
		{
			
			$q=getobj("survey_questions")->getquestion($x);
			if(!$q)
				$q="could not get question";
			$a=getj($row,"a$x");
			if(!$a)
				$a="could not get answer";			
			echo("<div style='margin-top:30px' class='section_head'>Question #$x</div>");
			echo("<div>$q</div>");
			echo("<div style='margin-top:10px' class='section_head'>Answer:</div>");
			echo("<div>$a</div>");
		}
        echo("</div>");  

	}

	public function printlist()
	{
		foreach ( $this->rows as $row )
		{
			$key=getj($row,'key');
				
			$leg=getobj("leg_list")->get_leg_by_key($key);
			if($leg)
			{
				
				$leg->print_list_row();
			}
			else {
				echo("<div>$key </div>");
				
			}
		}
	}
}



class canidate extends json_obj{
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

		$lastname=strtolower($this->get('last'));
		echo ("<div class='leg_bio' data-name='$lastname'><hr>");
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



class district extends json_obj{
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
class bill_list extends data_source
{
	public $list;
	public function __construct() {
		$this->get_data(1);
		$this->list=array();
		foreach ( $this->rows as $row )
		{
			if(getj($row,'year')=='Year')
				continue;
			$doc =$row->{	'gsx$doc' }->{'$t' };
			$this->list [$doc] = new bill ( $row );
		}		
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


class legislator extends json_obj{
	
	public $name;
	public $first;
	public $last;		
	public $displayname;
	public $id;
	public $key;	
	public $grade;
	public $score;
	
	public $grade_color;		
	public $uid;
	public $party;	
	public $chamberId;
	public $chamber;
	public $url_cover_jpg;

	public $jpg_path;
	public $url;

	public function __construct($data_in) {
		$this->data = $data_in;
		$this->first = $this->get('first');
		$this->last = $this->get('last');
		$this->key = $this->get('key');		
		
		
		$this->name = $this->first.' '.$this->last;
		$this->uid =$this->get('uid');
		$this->id = $this->get('id');
		$this->chamberId = $this->get('chamber');
		if($this->chamberId=='H')
			$this->chamber='House';
			else
			$this->chamber='Senate';
			
		$this->url ="http://www.ncleg.net/gascripts/members/viewMember.pl?sChamber=$this->chamber&nUserID=$this->uid";
		$this->create_grade();
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
		$votelist=array();
		$score=getobj("vote_data")->get_votes($this->key,$votelist);
		$grade=$this->get("grade");
		
			
		$color="#000";
		get_grade($score,$grade,$color);	
		$this->grade=$grade;	
		$this->grade_color=$color;
		$this->score=$score;
			
	}		
	public function print_table_val($label, $field) {
		$val = $this->get( $field);
		if(!$val)
			return;
		$this->print_table_row ( $label, $val );
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
		

		
		$lastname=strtolower($this->last);
		echo "<div class='leg_bio' data-name='$lastname'><hr/><div class='leg_thumb' >";
		echo "<a href='/guide/legpage.php?id=$this->key'>";
				

		echo "<img src='http://www.ncleg.net/$this->chamber/pictures/$this->uid.jpg'/></a>";
		$title=$this->get('title') ;
		echo "</div><div class='leg_info' ><a href='/guide/legpage.php?id=$this->key'><h2>$title $this->name</h2></a><table>";

		/*
		$district=
		*/
		$district=$this->get('district');
		$district_url="'/district.php?dist=". $district . "&ch=" . $this->chamberId . "'"; 
		$this->print_table_row ( 'District', "<a href=$district_url>$district</a>" );
		$running="Not running for re-election.";
		if($canidate)
		{
			$running=$canidate->get_running();
	
			
		}
		$this->print_table_row ( '2014 Election', $running );
		$this->print_table_val ( 'Party', 'party' );			
		$this->print_table_val ( 'Counties', 'county' );		
		$this->print_table_val ( 'Email', 'email' );		
		$this->print_table_val ( 'Phone', 'phone' );	
		if(option('grades'))
			$this->print_table_row ( 'Grade', $this->grade,$this->grade_color );

		$comment=$this->get("comment");
		if(!$comment)
		{
			$comment="Voting record, responsiveness to inquiries, feedback from other legislators and lobbyists, and public comment";
			$this->print_table_row ( 'Ranking Created By', $comment );
		}
		else 
			$this->print_table_row ( 'Reason For Grade', $comment );
		
	
				

		
		echo '</table>';
		echo ("<a target='_blank' href='$this->url'>Link to page on NCGA website</a>");
		echo "</div></div><div style='clear:both'></div>";
	}	
}



function grade_sort_func($a, $b) {
	if ($a->score == $b->score) {
		return 0;
	}
	return ($a->score < $b->score) ? 1 : -1;
}


class leg_list extends data_source{
	public $list;
	public function print_list() {
		echo "<div class='tbl_leglist' >";
		foreach ( $this->list as $d ) {
			$d->print_list_row ();
		}
		echo '</div>';
	}
	public function __construct() {
		$this->get_data(2);
		
	}
	public function get_leg_by_key($key) {
		foreach ( $this->rows as $row ) {
	
			if(
			(getj($row,'key')==$key))
			{
				return new legislator ( $row );
			}
		}
		return 0;
	}	
	public function get_leg_by_name($first,$last) {
		foreach ( $this->rows as $row ) {
				
			if(
				(getj($row,'first')==$first)&&
				(getj($row,'last')==$last))
			{
				return new legislator ( $row );
			}
		}
		return 0;
	}	
	public function get_leg_by_id($id) {
		foreach ( $this->rows as $row ) {
			
			$i = $row->{	'gsx$id' }->{'$t' };
			if ($i == $id) {
				return new legislator ( $row );
			}
		}
		return 0;
	}	
	public function get_leg_by_district($district) {
		foreach ( $this->rows as $row ) {
			$id = $row->{	'gsx$district' }->{'$t' };
			if ($district == $id) {
				return new legislator ( $row );
			}
		}
		return 0;
	}
	public function get_list($chamber,$district) {
		$this->list = array ();
		$sort=getParam("sort");

		foreach ( $this->rows as $row ) {
			$ch = $row->{	'gsx$chamber' }->{'$t' };
			if (($ch == 'chamber') || ($ch == ''))
				continue;
			if($chamber)
			{
				if($chamber != $ch)
					continue;
			
			}
			if($district)
			{
				if($district != $row->{	'gsx$district' }->{'$t' })
					continue;
					
			}	
			$legr=new legislator ( $row );
			
				/*
			$status = $row->{'gsx$status' }->{	'$t' };
			if ($status_filter)
				if (! in_array ( $status, $status_filter ))
					continue;
				*/
			$key= $row->{	'gsx$last' }->{'$t' };
			if($sort=='dist')
			{
				$key=$id = $row->{	'gsx$district' }->{'$t' };
				
				
			}

			$this->list [$key] = $legr;
		}
		if($sort=='grade')
		{
			uasort($this->list, 'grade_sort_func');
		}
		else
			ksort ( $this->list );
		return $this->list;
	}
}


?>

