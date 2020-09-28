<?php
/**
 * Created by PhpStorm.
 * User: dennis
 */

function olivia_common_header_images_form() {
  $form['data'] = array(
	'#type' => 'textarea',
	'#title' => 'Mapping',
	'#default_value' => variable_get('header_image_mapping', ''),
	'#description' => 'Enter in format URL-TO-THE-PAGE|NID',
  );
  
  $form['submit'] = array(
	'#type' => 'submit',
	'#value' => 'Save',
  );
  
  return $form;
}


function olivia_common_header_images_form_submit($form,&$form_state) {
  variable_set('header_image_mapping', $form_state['values']['data']);
  drupal_set_message('Your settings has been saved', 'status');
}

function olivia_common_get_nodes_for_special_pages() {
  $result = array();
  
  $special_pages = variable_get('header_image_mapping', '');
  $special_pages = explode(PHP_EOL, $special_pages);
  foreach($special_pages as $special) {
	$special = explode('|', $special);
	$result[trim($special['0'])] = trim($special['1']);
  }
  
  return $result;
}

function olivia_common_get_business_data($group_by_area = false) {
  $all = array();
  $filter = array();
  $top_levels = taxonomy_get_tree(3, 0, 1);

  foreach($top_levels as $top_level) {
	$sql = "SELECT n.nid, c.field_country_tid FROM {node} n ";
	$sql .= "INNER JOIN {field_data_field_country} c ON c.entity_id = n.nid ";
	$sql .= "INNER JOIN {field_data_field_area} a ON a.entity_id = n.nid ";
	$sql .= "WHERE n.type = 'business' AND n.status = 1 AND c.field_country_tid = " . $top_level->tid . " ORDER BY n.title ASC";
  
	$query = db_query($sql);
	foreach($query as $row) {
	  $c = taxonomy_term_load($row->field_country_tid);
	  $n = node_load($row->nid);
	  
	  if (!isset($filter[$c->tid])) {
		$filter[$c->tid]['name'] = $c->name;
	  }
	  
	  if (!isset($all[$c->tid])) {
		$all[$c->tid]['country_name'] = $c->name;
		$all[$c->tid]['description'] = $c->description;
		$all[$c->tid]['categories'] = array();
	  }
	
	  $areas = isset($n->field_area['und']) ? $n->field_area['und'] : false;

	  if ($areas) {
		foreach($areas as $area) {
		  $parents = taxonomy_get_parents($area['tid']);
		  if (count($parents) == 0) {
			$a = taxonomy_term_load($area['tid']);
			if (!isset($all[$c->tid]['categories'][$a->tid])) {
			  $all[$c->tid]['categories'][$a->tid]['name'] = $a->name;
			  $all[$c->tid]['categories'][$a->tid]['description'] = $a->description;
			  $all[$c->tid]['categories'][$a->tid]['image'] = isset($a->field_area_image['und']) ? image_style_url('area_image', $a->field_area_image['und']['0']['uri']) : FALSE;
			  $all[$c->tid]['categories'][$a->tid]['list'] = array();
			}
			
			$item = array();
			$item['nid'] = $n->nid;
			$item['title'] = $n->title;
			$item['logo'] = isset($n->field_logo['und']) ? image_style_url('map_logo', $n->field_logo['und']['0']['uri']) : false;
			$item['link'] = isset($n->field_homepage['und']) ? $n->field_homepage['und']['0']['value'] : false;
			$item['body'] = isset($n->body['und']) ? $n->body['und']['0']['value'] : '';
			$item['lat'] = $n->field_location['und']['0']['lat'];
			$item['lon'] = $n->field_location['und']['0']['lon'];
			$item['phone'] = FALSE;
			$item['email'] =  FALSE;
			$item['adress'] = FALSE;
			
			if (isset($n->field_telefon['und'])) {
			  $item['phone'] = $n->field_telefon['und']['0']['value'];
			}
			
			if (isset($n->field_e_post['und'])) {
			  $item['email'] = $n->field_e_post['und']['0']['value'];
			}
			
			if (isset($n->field_adress['und'])) {
			  $item['adress'] = $n->field_adress['und']['0']['value'];
			}
			
			$all[$c->tid]['categories'][$a->tid]['list'][] = $item;
		  }
		}
	  }
	}
  }

  return array('items' => $all, 'filter' => $filter);
}

function olivia_common_our_business_render() {
  $filter_list = array();

  $html = theme('olivia_common_our_business', array('filter_list' => $filter_list));

  return $html;
}

function olivia_common_header_render() {
  $items = array();
  $special_pages = olivia_common_get_nodes_for_special_pages();
  $path = implode('/', arg());

  if (arg(0) == "search") {
    $path = "search";
  }
  
  if (drupal_is_front_page()) {
    $query = db_query("SELECT n.nid FROM {node} n WHERE n.type = 'top_slider' AND n.status = 1 ORDER BY n.created DESC");
    foreach($query as $row) {
      $n = node_load($row->nid);
  
      $item = array(
        'image' => image_style_url('slider_image', $n->field_slider_image['und']['0']['uri']),
        'body' => isset($n->body['und']) ? $n->body['und']['0']['value'] : FALSE,
      );
  
      $items[] = $item;
    }
  } else if (arg(0) == "node" && (arg(1))) {
    $n = node_load(arg(1));
    if ($n) {
      if ($n->type == "page") {
        if (isset($n->field_header_image['und'])) {
          foreach($n->field_header_image['und'] as $img) {
            $item = array(
              'image' => image_style_url('slider_image_not_front', $img['uri']),
              'body' => FALSE,
            );
            
            $items[] = $item;
          }
        }
      }
    }
  } else if (isset($special_pages[$path])) {
    $n = node_load($special_pages[$path]);
    
    if (isset($n->field_header_image['und'])) {
      foreach($n->field_header_image['und'] as $img) {
        $item = array(
          'image' => image_style_url('slider_image_not_front', $img['uri']),
          'body' => FALSE,
        );
        
        $items[] = $item;
      }
    }
  }

  if (count($items) > 0) {
    $html = theme('olivia_common_header', array('items' => $items));
  
    return $html;
  }
}

function olivia_common_news_startpage_render() {
  $newsblock = views_embed_view('news', 'block_1');
  $article = false;
  $html = theme('olivia_common_news_startpage', array('news_block' => $newsblock, 'article' => $article));
  return $html;
}

/**
 * Build a list of board members to display in block
 */
function olivia_common_board_members_list_render() {
  global $language;

  $list = array();
  $query = db_query("SELECT n.nid, sh.field_staff_subheader_tid as subheader FROM node n 
  INNER JOIN field_data_field_staff_category sc ON sc.entity_id = n.nid AND sc.field_staff_category_tid = 26 
  INNER JOIN field_data_field_staff_subheader sh ON sh.entity_id = n.nid
  INNER JOIN taxonomy_term_data td ON td.tid = sh.field_staff_subheader_tid
  WHERE n.type = 'staff' AND n.language = '" .  $language->language  . "' ORDER BY td.weight ASC");

  foreach ($query as $row) {
    if (!isset($list[$row->subheader])) {
      $term = taxonomy_term_load($row->subheader);
      $list[$row->subheader]['name'] = $term->name;
      $list[$row->subheader]['list'] = array();
    }

    $board_member = node_load($row->nid);

    $img = isset($board_member->field_staff_img['und']) ? image_style_url('board_members', $board_member->field_staff_img['und']['0']['uri']) : false;

    if (!empty($board_member->field_staff_role['und'])) {
      $staff_role = taxonomy_term_load($board_member->field_staff_role['und'][0]['tid'])->name;
    }  else {
      $staff_role = false;
    }

    if (!empty($board_member->field_email_address['und'])) {
      $email = $board_member->field_email_address['und'][0]['email'];
    }  else {
      $email = false;
    }

    if (!empty($board_member->field_telephone['und'])) {
      $phone = $board_member->field_telephone['und'][0]['value'];
    } else {
      $phone = false;
    }

    if (!empty($board_member->body['und'])) {
      $body = $board_member->body['und'][0]['safe_value'];
    } else {
      $phone = false;
    }

    $person_info = array(
      'image' => $img,
      'name' => $board_member->title,
      'role' => $staff_role,
      'email' => $email,
      'telephone' => $phone,
	  'nid' => $board_member->nid,
      'body' => $body,
    );

    $list[$row->subheader]['list'][] = $person_info;
  }
  
  $html = theme('olivia_common_board_members_list', array(
    'board_people' => $list,
  ));

  return $html;
}

/**
 * Build a list of staff members to display in block
 */
function olivia_common_staff_members_list_render() {
  global $language;

  $staff_category_term = menu_get_object('taxonomy_term', 2);
  if (!isset($staff_category_term)) {
    // Try to get it without menu_get_object().
    if (arg(0) == 'taxonomy' && arg(1) == 'term' && arg(2)) {
      $staff_category_term = taxonomy_term_load(arg(2));
    }
  }

  if ($staff_category_term) {
    $tid = $staff_category_term->tid;
  } else {
    return false;
  }

  $list = array();
  $query = db_query("SELECT n.nid, sh.field_staff_subheader_tid as subheader FROM node n 
  INNER JOIN field_data_field_staff_category sc ON sc.entity_id = n.nid AND
  sc.field_staff_category_tid = '" .  $staff_category_term->tid  . "'
  INNER JOIN taxonomy_index ti ON ti.nid = n.nid AND ti.tid = " . $tid . "
  LEFT JOIN field_data_field_staff_subheader sh ON sh.entity_id = n.nid
  LEFT JOIN taxonomy_term_data td ON td.tid = sh.field_staff_subheader_tid
  WHERE n.type = 'staff' AND n.language = '" .  $language->language  . "' ORDER BY ti.weight ASC");

  foreach ($query as $row) {
    if (!isset($list[$row->subheader])) {
      $term = taxonomy_term_load($row->subheader);
      $list[$row->subheader]['name'] = $term->name;
      $list[$row->subheader]['list'] = array();
    }

    $staff_member = node_load($row->nid);

    $img = isset($staff_member->field_staff_img['und']) ? image_style_url('staff_members', $staff_member->field_staff_img['und']['0']['uri']) : false;
    $img_large = isset($staff_member->field_staff_img['und']) ? image_style_url('staff_members_large', $staff_member->field_staff_img['und']['0']['uri']) : false;
    
    if (!empty($staff_member->field_staff_role['und'])) {
      $staff_role = taxonomy_term_load($staff_member->field_staff_role['und'][0]['tid'])->name;
    }  else {
      $staff_role = false;
    }

    if (!empty($staff_member->field_email_address['und'])) {
      $email = $staff_member->field_email_address['und'][0]['email'];
    }  else {
      $email = false;
    }

    if (!empty($staff_member->field_telephone['und'])) {
      $phone = $staff_member->field_telephone['und'][0]['value'];
    } else {
      $phone = false;
    }

    if (!empty($staff_member->body['und'])) {
      $body = $staff_member->body['und'][0]['safe_value'];
    } else {
      $phone = false;
    }

    $person_info = array(
      'image' => $img,
      'large_image' => $img_large,
	  'nid' => $staff_member->nid,
      'name' => $staff_member->title,
      'role' => $staff_role,
      'email' => $email,
      'telephone' => $phone,
      'body' => $body,
    );

    $list[$row->subheader]['list'][] = $person_info;
  }

  $html = theme('olivia_common_staff_members_list', array(
    'staff_members' => $list,
  ));

  return $html;
}

/**
 * Function to create content to display in the block our_history
 */
function olivia_common_our_history_render() {
  global $language;
  $content = array();

  $query = db_query("SELECT n.nid FROM {node} n INNER JOIN field_data_field_event_date ed ON ed.entity_id = n.nid 
           WHERE n.type = 'our_history' AND n.status = 1
           AND n.language = '" .  $language->language  . "' ORDER BY ed.field_event_date_value ASC");

  foreach($query as $row) {
    $node = node_load($row->nid);

    $item = array(
      'id' => $node->nid,
      'title' => isset($node->title) ? $node->title : FALSE,
      'body' => isset($node->body['und']) ? $node->body['und']['0']['value'] : FALSE,
      'image' => isset($node->field_history_image['und']) ? image_style_url('history_image', $node->field_history_image['und']['0']['uri']) : FALSE,
    );

    $content[] = $item;
  }
  
  $html = theme('olivia_common_our_history', array(
    'content' => $content,
  ));

  return $html;
}


/**
 * Function to create content for a block called bottom_block
 * @return string
 */
function olivia_common_bottom_block_render() {
  $html = '';
  $path = implode('/', arg());

  //get the nodes to display
  $query = db_query("SELECT n.nid, pg.field_pages_value FROM {node} n INNER JOIN field_data_field_text_location tl ON tl.entity_id = n.nid 
          INNER JOIN field_data_field_pages pg ON pg.entity_id = n.nid WHERE n.type = 'block_pages' AND n.status = 1");

  foreach($query as $row) {

    if(drupal_match_path($path, $row->field_pages_value)) {
      $node = node_load($row->nid);

      $node_item = array(
        'title'=> isset($node->title) ? $node->title : false,
        'body' => isset($node->body['und']) ? $node->body['und']['0']['safe_value'] : false,
        'image' => isset($node->field_block_page_img['und']) ? image_style_url('block_pages', $node->field_block_page_img['und']['0']['uri']) : false,
        'text_location' => isset($node->field_text_location['und']) ? $node->field_text_location['und'][0]['value'] : false,
        'link' => array(
          'url' => isset($node->field_block_page_link['und']) ? $node->field_block_page_link['und'][0]['url'] : '',
          'title' => isset($node->field_block_page_link['und']) ? $node->field_block_page_link['und'][0]['title'] : '',
          'target' => isset($node->field_block_page_link['und']) && is_string($node->field_block_page_link['und'][0]['attributes']['target']) ? $node->field_block_page_link['und'][0]['attributes']['target'] : '',
        ),
      );

      $html .= theme('olivia_common_bottom_block', array(
        'content' => $node_item,
      ));
    }
  }

  if (strlen($html) > 0) {
    return $html;
  }

  return false;
}

function olivia_common_our_business_list_render() {
  $filter = array();
  $org_list = olivia_common_get_business_data(true);
  $list = $org_list['items'];
  $filter = $org_list['filter'];
  
  return theme('olivia_common_our_business_list', array('filter' => $filter, 'list' => $list));
}


/**
 * Helper function to find and render a block.
 */
function render_block_content($module, $delta) {
  $output = '';
  if ($block = block_load($module, $delta)) {
    if ($build = module_invoke($module, 'block_view', $delta)) {
      $delta = str_replace('-', '_', $delta);
      drupal_alter(array('block_view', "block_view_{$module}_{$delta}"), $build, $block);

      if (!empty($build['content'])) {
        return is_array($build['content']) ? render($build['content']) : $build['content'];
      }
    }
  }
  return $output;
}

function olivia_common_texthighlight($haystack, $needle) {
  $haystack=preg_replace("/($needle)/i","<strong>\${1}</strong>",$haystack);
  return $haystack;
}

function olivia_common_partition( $list, $p ) {
  $listlen = count( $list );
  $partlen = floor( $listlen / $p );
  $partrem = $listlen % $p;
  $partition = array();
  $mark = 0;
  for ($px = 0; $px < $p; $px++) {
    $incr = ($px < $partrem) ? $partlen + 1 : $partlen;
    $partition[$px] = array_slice( $list, $mark, $incr );
    $mark += $incr;
  }
  return $partition;
}

function olivia_common_search_autocomplete() {
  $key = arg(3) ? filter_xss(arg(3)) : false;
  $items = array();
  if ($key) {
    $key_org = $key;
    $key = mb_strtolower($key);
    $allowed_types = array('page', 'article', 'panel', 'webform');

    $query = db_query("SELECT n.nid FROM {node} n WHERE LOWER(n.title) LIKE '%" . $key . "%' AND n.status = 1 AND (n.type = 'page' OR n.type = 'article' OR n.type = 'panel' AND n.type = 'webform')");
    foreach($query as $row) {
      $n = node_load($row->nid);
      $item = array(
        'url' => url('node/' . $n->nid),
        'title' => olivia_common_texthighlight($n->title, $key_org),
      );

      $items[] = $item;
    }

    echo drupal_json_output($items);
  }
}