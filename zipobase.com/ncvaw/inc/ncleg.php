<?php

$g_bill_list=0;
$g_vote_data=0;

$refresh_data=array_key_exists ( "refresh", $_GET );

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
	public static $id = '0AonA9tFgf4zjdHhNd1FIeFJzVWRrdDlUangxWUlkTXc';
	public $rows;

	function get_data($tab) 
	{
		global $root;
		global $refresh_data;
		$filename=$root."/data/ncleg.json".$tab;
		if(! file_exists ( $filename ))	
			$refresh_data=1;
		if($refresh_data)
		{
			
			$url = "http://spreadsheets.google.com/feeds/list/0AonA9tFgf4zjdHhNd1FIeFJzVWRrdDlUangxWUlkTXc/$tab/public/values?alt=json";
			$file = file_get_contents ( $url );
			$fp = fopen ( $filename, 'w' );
			fwrite ( $fp, $file );
			fclose ( $fp );
		}
			
		$json = json_decode (  file_get_contents ( $filename ) );
		$this->rows = $json->{'feed' }->{'entry' };		
	}
}

class vote extends json_obj{
	public function __construct($data_in)
	{
		$this->data = $data_in;
	}
	public function print_vote_tr()
	{
	
		$vid=$this->get('vid');
		$doc= $this->get('doc');
		$vote= $this->get('vote');	

		$bill=get_bill_list()->get_bill($doc);
		

		echo "<tr>";
		echo "<td>$vid</td>";
		echo "<td>$doc</td>";
		echo "<td>$bill->official</td>";
		echo "<td>$vote</td>";
		echo "</tr>";
	}
}
class bill extends json_obj{
	public $name;
	public $stance;
	public $official;
	public $vid;
	public $desc;

	public function __construct($data_in)
	{
		$this->data = $data_in;
		$this->name =$this->get('name');
		$this->stance =$this->get('stance');
		$this->official =$this->get('official');
		$this->vid =$this->get('vid');
		$this->desc =$this->get('desc');
		
	}
}
class vote_data extends data_source
{
	public $list;
	public function __construct() {
		$this->get_data(2);

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
	public function print_list_votes($legid) {


		echo "<table class='votes'>";
		foreach ( $this->rows as $row )
		{
			if($legid == $row->{	'gsx$mid' }->{'$t' })
			{
				$vote=new vote($row);
				$vote->print_vote_tr();
			}
		}
		echo "</table>";
	}	
}
function get_vote_data()
{
	global $g_vote_data;
	if(!$g_vote_data)
		$g_vote_data=new vote_data();
	return $g_vote_data;
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
}

function get_bill_list()
{
	global $g_bill_list;
	if(!$g_bill_list)
		$g_bill_list=new bill_list();
	return $g_bill_list;
}
class legislator extends json_obj{
	
	public $name;
	public $displayname;
	public $id;
	public $uid;
	public $chamberId;
	public $chamber;
	public $url_cover_jpg;

	public $jpg_path;
	public $url;
	
	public function __construct($data_in) {
		$this->data = $data_in;
		$this->name = $this->get('name');
		$this->url =$this->get('url');
		$this->uid =$this->get('uid');
		$this->id = $this->get('id');
		$this->chamberId = $this->get('chamber');
		if($this->chamberId=='H')
			$this->chamber='House';
			else
			$this->chamber='Senate';
			
		
		
	}	
	public function print_list_votes() 
	{

		get_vote_data()->print_list_votes($this->id);
		

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
		global $isPhone;
		
		//$this->init ();
		if($isPhone)
			return $this->print_list_row_phone();
		
		
		echo "<tr class='leg_list'><td class='leg_thumb' >";
		echo "<a href='/guide/leg_page.php?id=$this->id'>";
				

		echo "<img src='http://www.ncleg.net/$this->chamber/pictures/$this->uid.jpg'/></a>";
		
		echo "</td><td class='td_doglist_info' ><h2 >$this->name</h2><table class='doginfo'>";
		
		$this->print_table_val ( 'District', 'district' );		
		$this->print_table_val ( 'County', 'county' );		
		$this->print_table_val ( 'Email', 'email' );		
		$this->print_table_val ( 'Phone', 'phone' );		
				

		
		echo '</table>';
		echo ("<a target='_blank' href='$this->url'>Link to page on NCGA website</a>");
		echo '</td></tr>';
	}	
}






class leg_list extends data_source{
	public $list;
	public function print_list() {
		echo "<table class='tbl_doglist' border='1'>";
		foreach ( $this->list as $d ) {
			$d->print_list_row ();
		}
		echo '</table>';
	}
	public function __construct() {
		$this->get_data(3);
		
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
		
		foreach ( $this->rows as $row ) {
			$name = $row->{	'gsx$name' }->{'$t' };
			if (($name == 'name') || ($name == ''))
				continue;
			if($chamber)
			{
				if($chamber != $row->{	'gsx$chamber' }->{'$t' })
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
			$this->list [$name] = new legislator ( $row );
		}
		ksort ( $this->list );
		return $this->list;
	}
}



?>

