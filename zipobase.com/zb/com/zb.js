/*
             Copyright 2007, ZipoSoft, Inc. (Acme Device Drivers, Inc.)                    
                                                                         
 This file is part of ZipoBase, from ZipoSoft. See ZipoBase.com for more infomation                                                                            
                                                                             
 THIS CODE IS PROVIDED "AS IS". ZIPOSOFT MAKES NO WARRANTIES, EXPRESSED,
 IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   
*/

function tl_row_hi(obj)
{
  obj.style.backgroundColor  = '#ffff80';
  obj.style.color  = 'blue';
}
function tl_row_low(obj)
{
  obj.style.backgroundColor  = 'white';
  obj.style.color  = 'black';
}


var count=0;


function showsubmenu(elm,ev) 
{
    elm.className = 'misub_hi';
    
    showmenu(elm,ev);
}

function _hi(elm) 
{
    elm.className = elm.className+'_hi';
}
function _low(elm) 
{
    elm.className = elm.className.replace(/_hi/g,"");
}
function hidesubmenu(elm,ev) 
{
    elm.className = 'misub';
    hidemenu(elm,ev);
}
function mi_low(elm) 
{
  elm.className = 'mi';
  var lists = elm.getElementsByTagName('a');
  if(lists.length>0)
  {
       lists[0].className = "mi";
  }
}
function mi_hi(elm) 
{
  elm.className = 'mi_hi';
  var lists = elm.getElementsByTagName('a');
  if(lists.length>0)
  {
       lists[0].className = "mi_hi";
  }
}

function showhide(elmid,show)
{
    var y=document.getElementById(elmid);
    if(y) 
    {
    if(show===true)
    {
        y.style.display="";
        y.style.visibility="visible";
     }
     else
     {
        y.style.display="none";
        y.style.visibility="hidden";
     }
 }    
}

function load()
{    
  DragDrop.init();
}


function go(obj,ev,link)
{
    location.href=link;
    ev.stopPropagation();
    
    window.event.cancelBubble = true;
    return false;
}
function selectsubmit(elmid,act)
{
    var selform=document.getElementById(elmid);
    var x= 'act' + elmid ;
    var y=document.getElementById(x);
    if(y) 
    {
        y.value=act;
    }
    
    selform.submit();
}
