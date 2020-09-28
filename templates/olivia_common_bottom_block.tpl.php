<div class="block-content">
  <div class="main-content">
    <?php if($content['image']): ?>
      <div class="image">
        <div class="overlay"></div>
        <img src="<?php print $content['image']; ?>"  />
      </div>
    <?php endif; ?>
    <?php if($content['body'] || $content['title'] || $content['link']): ?>
      <div class="text-wrapper <?php echo $content['text_location']; ?>">
        <div class="inner">
          <div class="placement">
            <h4 class="node-title"><?php print $content['title']; ?></h4>
            <div class="body"><?php print $content['body']; ?></div>
            <?php if (strlen($content['link']['url']) > 0) { ?>
              <div class="link">
                <a href="<?php print $content['link']['url']; ?>" target="<?php print $content['link']['target']; ?>"
                   class="btn blue"><?php print $content['link']['title']; ?></a>
              </div>
            <?php } ?>
          </div>
        </div>
      </div>
    <?php endif; ?>
  </div>
</div>