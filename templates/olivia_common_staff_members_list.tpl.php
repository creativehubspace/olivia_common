<?php if (count($staff_members) > 0) { ?>
  <?php foreach($staff_members as $key => $row) { ?>
    <h3 class="grouping-header"><?php print $row['name']; ?></h3>
    <div class="group row-num-<?php print count($row['list']); ?>">
      <?php $i = 0; ?>
      <?php $i2 = 0; ?>
      <?php foreach($row['list'] as $person) { ?>
        <?php $i++; ?>
        <?php $i2++; ?>
        <?php $count = count($row['list']); ?>
        <?php if ($i2 == 1) { ?>
          <div class="g">
        <?php } ?>
        <?php if (($i % 3) == 0 || $i == $count) { ?>
          <div class="container last">
        <?php } else { ?>
          <div class="container">
        <?php } ?>
          <div class="person-info" id="person-<?php echo $person['nid']; ?>">
            <div class="desc"><?php print $person['body']; ?></div>
            <?php if($person['image'] || $person['large_image']) { ?>
              <div class="image">
                <?php if($person['image']) { ?>
                  <img src="<?php echo $person['image']; ?>" alt="" />
                <?php } ?>
                <?php if($person['large_image']) { ?>
                  <img data-image="large" src="<?php echo $person['large_image']; ?>" alt="" />
                <?php } ?>
              </div>
            <?php } ?>
            <?php if($person['name'] || $person['role']) { ?>
              <div class="title-info">
                <?php if($person['name']) { ?>
                  <div class="name"><?php print $person['name']; ?></div>
                <?php } ?>
                <?php if($person['role']) { ?>
                  <div class="role"><?php print $person['role']; ?></div>
                <?php } ?>
              </div>
            <?php } ?>
              <?php if($person['email'] || $person['telephone']) { ?>
                <div class="contact-info">
                  <?php if ($person['email']) { ?>
                    <div class="email"><a href="mailto:<?php print $person['email']; ?>"><?php print $person['email']; ?></a></div>
                  <?php } ?>
                  <?php if ($person['telephone']) { ?>
                    <div class="telephone"><?php print $person['telephone']; ?></div>
                  <?php } ?>
                </div>
              <?php } ?>
          </div>
          <div class="mobile-modal-content">
          </div>
        </div>
        <?php if (($i % 3) == 0 || $i == $count) { ?>
          <div class="modal-content">
          </div>
          </div>
          <?php $i2 = 0; ?>
        <?php } ?>
      <?php } ?>
    </div>
  <?php } ?>
<?php } ?>
