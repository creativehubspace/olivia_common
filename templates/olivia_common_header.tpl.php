<?php if (count($items) > 0) { ?>
  <div class="items top-slider">
    <?php if (count($items) > 1) { ?>
      <div class="controller-wrapper prev">
        <a href="javascript:;" class="controller prev">Föregående</a>
      </div>
    <?php } ?>
    <?php foreach($items as $item) { ?>
       <div class="item">
         <img src="<?php echo $item['image']; ?>" alt="" />
         <?php if ($item['body']) { ?>
           <div class="text-wrapper">
             <div class="inner">
                <div class="text-box">
                  <?php echo $item['body']; ?>
                </div>
             </div>
           </div>
         <?php } ?>
       </div>
    <?php } ?>
    <?php if (count($items) > 1) { ?>
      <div class="controller-wrapper next">
        <a href="javascript:;" class="controller next">Nästa</a>
      </div>
    <?php } ?>
  </div>
  <?php if (count($items) > 1) { ?>
    <div class="pager-wrapper"></div>
  <?php } ?>
<?php } ?>