<?php if(count($content) > 0) { ?>
  <div class="dotted-line"></div>
  <div class="block-content">
    <?php $i = 0; ?>
    <?php foreach($content as $row => $node_content) { ?>
    <?php $i++; ?>
    <?php $count = count($content);?>
      <?php if($row == 0) { ?>
        <div class="row-first content-wrapper <?php print $node_content['image'] ? 'has-image': ''; ?> <?php print $i == $count ? 'last': ''; ?> row-<?php print $i ?>" id="history-<?php print $node_content['id']; ?>">
      <?php } elseif($row % 2 == 0) { ?>
        <div class="row-even content-wrapper <?php print $node_content['image'] ? 'has-image': ''; ?> <?php print $i == $count ? 'last': ''; ?> row-<?php print $i ?>" id="history-<?php print $node_content['id']; ?>">
          <?php } else { ?>
          <div class="row-odd content-wrapper <?php print $node_content['image'] ? 'has-image': ''; ?> <?php print $i == $count ? 'last': ''; ?> row-<?php print $i ?>" id="history-<?php print $node_content['id']; ?>">
      <?php } ?>
        <div class="border-line"></div>
        <?php if($row == 0) { ?>
            <div class="row row-first node-<?php print $node_content['id']; ?> <?php print $classes; ?> <?php print $i == $count ? 'last': ''; ?>">
              <?php if ($node_content['image']) { ?>
                <div class="image">
                  <img src="<?php print $node_content['image']; ?>"  />
                </div>
              <?php } ?>
                <div class="main-content <?php print $node_content['image'] ? 'has-image': ''; ?>">
                  <h2 class="node-title"><?php print $node_content['title']; ?> </h2>
                  <div class="body-text"><?php print $node_content['body']; ?></div>
                </div>
            </div>
        <?php } elseif($row % 2 == 0) { ?>
          <div class="row row-even node-<?php print $node_content['id']; ?> <?php print $classes; ?> <?php print $i == $count ? 'last': ''; ?>">
            <?php if ($node_content['image']) { ?>
              <div class="image">
                <img src="<?php print $node_content['image']; ?>"  />
              </div>
            <?php } ?>
            <div class="main-content <?php print $node_content['image'] ? 'has-image': ''; ?>">
              <h2 class="node-title"><?php print $node_content['title']; ?> </h2>
              <div class="body-text"><?php print $node_content['body']; ?></div>
            </div>
          </div>
        <?php } else { ?>
          <div class="row row-odd node-<?php print $node_content['id']; ?> <?php print $classes; ?> <?php print $i == $count ? 'last': ''; ?>">
            <?php if ($node_content['image']) { ?>
              <div class="image">
                <img src="<?php print $node_content['image']; ?>"  />
              </div>
            <?php } ?>
            <div class="main-content <?php print $node_content['image'] ? 'has-image': ''; ?>">
              <h2 class="node-title"><?php print $node_content['title']; ?> </h2>
              <div class="body-text"><?php print $node_content['body']; ?></div>
            </div>
          </div>
        <?php } ?>
      </div>
    <?php } ?>
  </div>
<?php } ?>
