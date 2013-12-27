<?php

?>

<table style="height='100%'">
	<tr>
		<td valign="top" style="padding: 0px">
			<?php photo_set_print_thumbs($photos); ?>
			<div id="pleasewait">
				<b>Please wait for photos to load...</b>
			</div>
       </td>
		<td id="photopane" valign="top" onmouseover="on_mouse_out_thumbs()"	style="padding: 0px">
            <?php 
            photo_set_print_imgs($photos,$titles);

             
            ?>
            
		</td>
	</tr>
</table>
