<?php
/**
 * Created by PhpStorm.
 * User: dennis
 */

define("UNIQUEKEY", "");
define("LIMIT", "100");
define("ITEMSPERREQUEST", "100");

// set feed URL

function olivia_common_get_press_release_data()
{
    
    
    $url = 'http://www.mynewsdesk.com/services/pressroom/list/ac0923e8dc2db44f1a4a8a4b636e2507/?type_of_media=pressrelease&limit=100&offset=0&order=published&format=json';

    $content = file_get_contents($url);
    $json = json_decode($content, true);
    $total_count = $json['items']['total_count'];
    $limit = 100;
    $ItemsPerRequest = 100;
    
    if  ($total_count > $limit)
    {
        
        $variables_array = array();
        
        $total_requests = intval($total_count/$limit);
        
        //add one more in requests 
        if($total_count % $limit > 0) {
            $total_requests = $total_requests + 1;
        }
        
        for($i=1; $i<=$total_requests; $i++)
        {
            $offset = ($i - 1) * $ItemsPerRequest;
            
            $url = "http://www.mynewsdesk.com/services/pressroom/list/ac0923e8dc2db44f1a4a8a4b636e2507/?type_of_media=pressrelease&limit=".$limit."&offset=".$offset."&order=published&format=json";
            $content = file_get_contents($url);
            $json = json_decode($content, true);
            
            $variables_array =  array_merge($variables_array, get_pressrelease_variables_array($json));
        }
    }
    else
    {
        $variables_array = get_pressrelease_variables_array($json);
    }
    
   return isset($variables_array) ? $variables_array : array();
}

// then you can do

function get_pressrelease_variables_array($json)
{
    //Loop through all the members of the Item array

    foreach($json['items']['item'] as $key => $Item)
    {
        //Now you can access the 'row' data using $Item in this case
      
       $tags = isset($Item['tags']) ? fetch_pressrelease_values($Item['tags']) : array();
       $subjects = isset($Item['subjects']) ? fetch_pressrelease_values($Item['subjects']) : array();
       $related_items = isset($Item['related_items']) ? fetch_pressrelease_values($Item['related_items']) : array();
       $variables_array[$key] = array(
            "id" => $Item['id'],
            "url" => $Item['url'],
            "published_at" => strtotime($Item['published_at']),
            "created_at" => strtotime($Item['created_at']),
            "updated_at" => strtotime($Item['updated_at']),
            "header" => $Item['header'],
            "image" => isset($Item['image']) ? $Item['image'] : false,
            "summary" => str_replace('<br>', '', str_replace('<br />', '', $Item['summary'])),
            "body" => $Item['body'],
            "boilerplate" => $Item['boilerplate'],
            "tags" => $tags,
            "subjects" => $subjects,
            "contact_people" => array("contact_person" => fetch_pressrelease_contact_people($Item['contact_people'])),
            "related_items" => $related_items
        );
       
        if (count($related_items) > 0) {
            foreach($related_items as $r) {
                if ($r['type_of_media'] == 'contact_person') {
                    $has_nid = intval(db_query("SELECT n.nid FROM {node} n INNER JOIN {field_data_field_contact_person_id} c ON c.entity_id = n.nid AND c.bundle = 'staff' WHERE c.field_contact_person_id_value = " . $r['item_id'])->fetchField());
                   
                    if ($has_nid > 0) {
                        $variables_array[$key]['contact_people_nids'][] = $has_nid;
                    }
                }
            }
        }   
    }
     
    return isset($variables_array) ? $variables_array : array();
}

function  fetch_pressrelease_contact_people($contacts)
{
    
    foreach($contacts as $contact)
    {
        $contacts_array = array(
          "id" => $contact['id'],
          "url" => $contact['url'],
          "published_at" => $contact['published_at'],
          "created_at" => $contact['created_at'],
          "updated_at" => $contact['updated_at'],
          "position" => $contact['position'],
          "name" => $contact['name'],
          "title" => $contact['title'],
          "specialist" => $contact['specialist'],
          "phone" => $contact['phone'],
          "phone_alternative" => $contact['phone_alternative'],
          "email" => $contact['email'],
          "pressroom_contact" => $contact['pressroom_contact'],
          "summary" => $contact['summary'],
          "body" => $contact['body'],
          "image" => $contact['image'],
          "image_small" => $contact['image_small'],
          "image_medium" => $contact['image_medium'],
          "image_thumbnail_large" => $contact['image_thumbnail_large'],
          "image_thumbnail_medium" => $contact['image_thumbnail_medium'],
          "image_thumbnail_small" => $contact['image_thumbnail_small']
        );
    }
    
    return isset($contacts_array) ? $contacts_array : array();
}

function fetch_pressrelease_values($values)
{
    foreach($values as $key => $value)
    {
        $values_array[$key] = $value;
    }
    
    return $values_array;
}

?> 