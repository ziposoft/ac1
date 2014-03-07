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
		echo "<td>";
		if($picture) 
			echo "<div class='$class'>$vote</div><img style='width:60px' title='$title' src='/img/$picture'/>";
		
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
	public function __construct($data_in)
	{
		$this->data = $data_in;
		$this->nickname =$this->get('nickname');
		$this->stance =$this->get('stance');
		if($this->stance == 'pro')
			$this->effect= "supports animal welfare";
		else 
			$this->effect= 	"is harmful to animal welfare";
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
			return "Supports animal welfare";
		return 	"Harmful to animal welfare";
	}
	public function print_tr()
	{
		echo "<tr><td>$this->year</td>";
		echo "<td><h3><a href='/guide/billpage.php?doc=$this->doc'>$this->doc</a></h3>";
	
		echo "<div><a href='/guide/billpage.php?doc=$this->doc'>$this->official</a> </div>";
		echo "<p>$this->effect </p>";
		echo "<p><a target='_blank' href='http://www.ncleg.net/gascripts/BillLookUp/BillLookUp.pl?Session=$this->year&BillID=$this->doc&submitButton=Go' >Link to $this->doc on NCLEG.NET</a> </p>";
		echo "<p>$this->desc </p></td></tr>";
	}
	public function print_page()	
	{
		echo "<H1>$this->year - $this->doc</H1>";
		echo "<H2>$this->official </H2>";
		
		
		echo "<p>$this->effect </p>";		
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
				$mid= $row->{	'gsx$mid' }->{'$t' };
				$leg=getobj("leg_list")->get_leg_by_id($mid);
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
			echo "<a href='/guide/legpage.php?id=$leg->id'>$leg->name</a>";
			$comma=1;
		}
		echo("</div>");
	}	
	public function get_votes($legid,&$list) {
		$score=0;
		foreach ( $this->rows as $row )
		{
			if($legid == getj($row,'mid'))
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

class canidate extends json_obj{
	public $displayname;

	public function __construct($data_in) {
		$this->data = $data_in;
		$this->displayname=$this->get('nameonballot');
	}
	public function print_table_val($label, $field) {
		$val = $this->data->{	'gsx$' . $field }->{'$t' };
		if(!$val)
			return;
		$this->print_table_row ( $label, $val );
	}	
	public function print_table_row($label, $val) {
		echo "<tr><td class='leg_info_label'>$label: </td><td class='leg_info_val'>$val</td></tr>";
	}	
	public function print_list_row() {
	
		echo "<tr ><td class='leg_thumb' >";
		echo "<a href=''>";
		echo "<img src=''/></a>";
		echo "</td><td class='leg_info' ><a href=''><h2> $this->displayname</h2></a><table>";
	
		$this->print_table_val ( 'District', 'district' );
		$this->print_table_val ( 'Party', 'party' );
				//$this->print_table_val ( 'Counties', 'county' );
				//$this->print_table_val ( 'Email', 'email' );
				//$this->print_table_val ( 'Phone', 'phone' );
		echo '</table>';
		echo '</td></tr>';	
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
	public function printresp($first,$last)
	{
		$row = $this->getrow($first,$last);
		if(!$row)
			return;
		
		for ($x=1; $x<=5; $x++)
		{
			
			$q=getobj("survey_questions")->getquestion($x);
			if(!$q)
				$q="could not get question";
			$a=getj($row,"a$x");
			if(!$a)
				$a="could not get answer";			
			echo("<div>Question #$x</div>");
			echo("<div>$q</div>");
			echo("<div>Answer:</div>");
			echo("<div>$a</div>");
		}

	}

	public function printlist()
	{
		foreach ( $this->rows as $row )
		{
			$first=getj($row,'firstname');
			$last=getj($row,'lastname');
				
			$leg=getobj("leg_list")->get_leg_by_name($first,$last);
			if($leg)
			{
				
				$leg->print_list_row();
			}
		}
	}
}
class canidates extends data_source
{
	public function __construct() {
		$this->get_data(5);
		$this->list=array();
		
	}	
	public function getlist($ch,$num)
	{
		$set=array();
		foreach ( $this->rows as $row )
		{
			if( (getj($row,'district')==$num) &&(getj($row,'chamber')==$ch))
			{
				$set [] = new canidate ( $row );
				
			}	
			
		}
		return $set;
	}
	
	public function printlist($ch,$num=null)
	{
		echo "<table class='tbl_leglist' >";
	
		foreach ( $this->rows as $row )
		{
			
			if(getj($row,'chamber')==$ch)
				if( getj($row,'district')==$num)
				{
					$x = new canidate ( $row );
					$x->print_list_row();
					
					
				}	
			
		}
		echo '</table>';
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
		echo "<table class='bills'>";
		foreach ( $this->list as $bill )
		{
			$bill->print_tr();
		}
		echo "</table>";	
		
	}	
}

function get_grade($score,&$grade,&$color)
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
/*
	-2 => "D-",
	-1 => "D",
	0 => "C",
	1 => "C+",
	2 => "B",
	3 => "B+",
	4 => "A-",
	5 => "A",
	6 => "A+",*/
	];	
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
	
	
	
}
class legislator extends json_obj{
	
	public $name;
	public $first;
	public $last;		
	public $displayname;
	public $id;
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
		
		
		
		$this->name = $this->first.' '.$this->last;
		$this->uid =$this->get('uid');
		$this->id = $this->get('id');
		$this->chamberId = $this->get('chamber');
		if($this->chamberId=='H')
			$this->chamber='House';
			else
			$this->chamber='Senate';
			
		$this->url ="http://www.ncleg.net/gascripts/members/viewMember.pl?sChamber=$this->chamber&nUserID=$this->uid";
		
	}	
	
	public function print_survey() {
		getobj("survey_data")->printresp($this->first ,$this->last);
	}	
	public function print_list_votes() 
	{
		getobj("vote_data")->print_list_votes($this->id,0);

	}
	public function print_list_sponsorship()
	{
		getobj("vote_data")->print_list_votes($this->id,1);
	
	}	
	public function print_table_val($label, $field) {
		$val = $this->data->{	'gsx$' . $field }->{'$t' };
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
		
		$votelist=array();
		//$this->init ();
		/*
		if($isPhone)
			return $this->print_list_row_phone();
			*/
		
		$score=getobj("vote_data")->get_votes($this->id,$votelist);
		
		
		$lastname=strtolower($this->last);
		echo "<div class='leg_bio' data-name='$lastname'><hr/><div class='leg_thumb' >";
		echo "<a href='/guide/legpage.php?id=$this->id'>";
				

		echo "<img src='http://www.ncleg.net/$this->chamber/pictures/$this->uid.jpg'/></a>";
		$title=$this->get('title') ;
		echo "</div><div class='leg_info' ><a href='/guide/legpage.php?id=$this->id'><h2>$title $this->name</h2></a><table>";
		$grade="?";
		$color="#000";
		get_grade($score,$grade,$color);
		$this->print_table_val ( 'District', 'district' );	
		$this->print_table_val ( 'Party', 'party' );			
		$this->print_table_val ( 'Counties', 'county' );		
		$this->print_table_val ( 'Email', 'email' );		
		$this->print_table_val ( 'Phone', 'phone' );	
		if(option('grades'))
			$this->print_table_row ( 'Grade', $grade,$color );
				

		
		echo '</table>';
		echo ("<a target='_blank' href='$this->url'>Link to page on NCGA website</a>");
		echo "</div></div><div style='clear:both'></div>";
	}	
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
				/*
			$status = $row->{'gsx$status' }->{	'$t' };
			if ($status_filter)
				if (! in_array ( $status, $status_filter ))
					continue;
				*/
			$key=$id = $row->{	'gsx$district' }->{'$t' };
			if($sort!='dist')
			{
				$key= $row->{	'gsx$last' }->{'$t' };
				
			}

			$this->list [$key] = new legislator ( $row );
		}
		ksort ( $this->list );
		return $this->list;
	}
}


?>

