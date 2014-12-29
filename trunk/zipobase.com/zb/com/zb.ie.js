/*
             Copyright 2007, ZipoSoft, Inc. (Acme Device Drivers, Inc.)                    
                                                                         
 This file is part of ZipoBase, from ZipoSoft. See ZipoBase.com for more infomation                                                                            
                                                                             
 THIS CODE IS PROVIDED "AS IS". ZIPOSOFT MAKES NO WARRANTIES, EXPRESSED,
 IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   
*/



function stopprop(notused)
{
    window.event.cancelBubble = true;
}

function hidemenu(elm,ev) 
{
 //if(console)  console.log("hide %s ", ev.target.className);
  var lists = elm.getElementsByTagName('table');
 
  if(lists.length>0)
  {
       lists[0].style.display = "none";
    // if(console)  console.log("hide %s %d %s ",elm.className,count++, ev.target.className);
 }
 
}
function showmenu(elm,ev) 
{


  var lists = elm.getElementsByTagName('table');
  
  if(lists.length>0)
  {
       lists[0].style.display = "inline";

  }
  
}