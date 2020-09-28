<?php if (count($board_people) > 0) { ?>
  <?php foreach($board_people as $key => $row) { ?>
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
        <div class="person-info" id="person-<?php echo $person['nid']; ?>">
          <div class="desc"><?php print $person['body']; ?></div>
          <div class="image">
            <?php if($person['image']) { ?>
            <img src="<?php echo $person['image']; ?>" alt="" />
            <?php } ?>
          </div>
          <div class="staff-info">
            <div class="title-info">
              <?php if($person['name']) { ?>
                <div class="name"><?php print $person['name']; ?></div>
              <?php } ?>
              <?php if($person['role']) { ?>
              <div class="role"><?php print $person['role']; ?></div>
              <?php } ?>
            </div>
            <div class="contact-info">
              <?php if ($person['email']) { ?>
                <div class="email"><a href="mailto:<?php print $person['email']; ?>"><?php print $person['email']; ?></a></div>
              <?php } ?>
              <?php if ($person['telephone']) { ?>
                <div class="telephone"><?php print $person['telephone']; ?></div>
              <?php } ?>
            </div>
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
