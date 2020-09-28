<?php
  $filter_area = isset($_REQUEST['area']) ? $_REQUEST['area'] : false;
  $filter_country = isset($_REQUEST['country']) ? $_REQUEST['country'] : '5';
  $current_url = implode('/' , arg());
  $country_term = taxonomy_term_load($filter_country);
?>
<div class="business-wrapper">
  <?php if (count($filter) > 0) { ?>
    <ul class="country-filter" id="country-filter-area">
      <?php foreach($filter as $tid => $name) { ?>
        <li><a href="javascript:;" data-c="<?php echo $tid; ?>"><?php echo $name['name']; ?></a></li>
      <?php } ?>
    </ul>
  <?php } ?>
  
  <?php if (count($list) > 0) { ?>
    <div class="list">
      <?php $c = 0; ?>
      <?php foreach($list as $key => $data) { ?>
        <?php  $c++; ?>
        <div class="box-container <?php echo $c == 1 ? 'first-item' : ''; ?> c-<?php echo $key; ?>">
            <h2 class="country-header"><?php echo $data['country_name']; ?></h2>
            <?php if (strlen($data['description']) > 0) { ?>
              <div class="text ingress">
                <?php echo $data['description']; ?>
              </div>
            <?php } ?>
            <?php $categories = $data['categories']; ?>
            <?php if (count($categories) > 0) { ?>
              <?php foreach($categories as $category) { ?>
                <div class="box">
                  <div class="image">
                    <?php if ($category['image']) { ?>
                      <img src="<?php echo $category['image']; ?>" alt="<?php echo $category['name']; ?>" />
                    <?php } ?>
                  </div>
                  <div class="description">
                    <h2><?php echo $category['name']; ?></h2>
                    <?php echo $category['description']; ?>
                    <a class="view-list" href="javascript:;">Se bolag inom <?php echo $category['name']; ?><span></span></a>
                  </div>
               </div>
              <div class="list-of-companys-container">
                <div class="list-of-companys">
                  <?php
                    $companys = $category['list'];
                    $total_items = round((count($category['list'])/3), 0);
                    $l2 = olivia_common_partition($category['list'], 3);
                  ?>
                  <?php foreach($l2 as $l_key => $l_value) { ?>
                      <div class="b-items">
                        <ul>
                          <?php foreach($l_value as $l_item) { ?>
                            <li>
                              <a href="javascript:;" class="toggle-info-window" id="verksamhet-<?php echo $l_item['nid']; ?>"  data-nid="<?php echo $l_item['nid']; ?>">> <?php echo $l_item['title']; ?></a>
                              
                              <div class="business-window business-window-list">
                                <div class="info">
                                  <div class="l">
                                    <div class="logo-wrapper">
                                      <?php if ($l_item['logo']) { ?>
                                        <img src="<?php echo $l_item['logo']; ?>" alt="<?php echo $l_item['title']; ?>">
                                      <?php } ?>
                                    </div>
                                    <?php if ($l_item['link']) { ?>
                                      <a href="<?php echo $l_item['link']; ?>" target="_blank" class="open-webpage">Öppna hemsida</a>
                                    <?php } ?>
                                  </div>
                                  <div class="r">
                                    <h2><?php echo $l_item['title']; ?></h2>
                                    <div class="t">
                                      <?php if ($l_item['body']) { ?>
                                        <?php echo $l_item['body']; ?>
                                      <?php } ?>
                                      <div class="row">
                                        <label>Kontaktuppgifter:</label>
                                        <?php if ($l_item['phone']) { ?>
                                          <span class="phone"><?php echo $l_item['phone']; ?></span>
                                        <?php } ?>
                                        <?php if ($l_item['email']) { ?>
                                          <span class="email"><a href="mailto:<?php echo $l_item['email']; ?>"><?php echo $l_item['email']; ?></a></span>
                                        <?php } ?>
                                        
                                        <?php if ($l_item['adress']) { ?>
                                          <span class="adress"><?php echo $l_item['adress']; ?></span>
                                        <?php } ?>
                                      </div>
                                    </div>
                                  </div>
                                </div>
                              </div>
                            </li>
                          <?php } ?>
                        </ul>
                      </div>
                    <?php } ?>
                </div>
              </div>
              <?php } ?>
            <?php } ?>
        </div>
      <?php } ?>  
    </div>
  <?php } ?>
</div>