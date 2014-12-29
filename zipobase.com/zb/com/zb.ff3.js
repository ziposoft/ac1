/*
             Copyright 2007, ZipoSoft, Inc. (Acme Device Drivers, Inc.)                    
                                                                         
 This file is part of ZipoBase, from ZipoSoft. See ZipoBase.com for more infomation                                                                            
                                                                             
 THIS CODE IS PROVIDED "AS IS". ZIPOSOFT MAKES NO WARRANTIES, EXPRESSED,
 IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   
*/

function stopprop(e)
{
    e.stopPropagation();
}
function rowgo(obj,e,link)
{
    e.stopPropagation();
    if((e.target.tagName=="tr")||(e.target.tagName=="TR"))
    {
    location.href=link;
    return false;
    }
    return true;
}
function hidemenu(elm,ev) 
{
    if(
        (ev.target.className == 'menutop')||
        (ev.target.className == 'menu')||
        (ev.target.className == 'misub')||
        (ev.target.className == 'mi')||
        (ev.target.className == 'menusub')
    )
    {
        var lists = elm.getElementsByTagName('table');

        if(lists.length>0)
        {
            lists[0].style.display = "none";
            //  if(console)  console.log("hide %s %d %s ",elm.className,count++, ev.target.className);
        }
    }
   // if(console)  console.log("hide %s %s", ev.target,ev.target.className);
}
function showmenu(elm,ev) 
{

  var othermenus = document.getElementsByClassName('menu');
  var oElement;
  for(var i=0; i<othermenus.length; i++){
        othermenus[i].style.display="none";    
        }
        
         
 
  var lists = elm.getElementsByTagName('table');
  
  if(lists.length>0)
  {
       lists[0].style.display = "inline";

  }
  
}