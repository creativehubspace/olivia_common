(function($) {
  var ajax_requests = new Array();
  var map;
  var hash = false; 
  
  
  function checkHash(){ 
    hash = window.location.hash; 
    processHash(hash); 
  }
  
  checkHash();
  
  function processHash(hash){
    $(document).ready(function() {
      var offset = $('.header').outerHeight();
      if (hash.length > 0) {
        $('html, body').animate({
          scrollTop: $(hash).offset().top-offset
        }, 1000);
      }
    });
  }
  
  function createCookie(name, value, days) {
      var expires;
  
      if (days) {
          var date = new Date();
          date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
          expires = "; expires=" + date.toGMTString();
      } else {
          expires = "";
      }
      document.cookie = encodeURIComponent(name) + "=" + encodeURIComponent(value) + expires + "; path=/";
  }
  
  function readCookie(name) {
      var nameEQ = encodeURIComponent(name) + "=";
      var ca = document.cookie.split(';');
      for (var i = 0; i < ca.length; i++) {
          var c = ca[i];
          while (c.charAt(0) === ' ') c = c.substring(1, c.length);
          if (c.indexOf(nameEQ) === 0) return decodeURIComponent(c.substring(nameEQ.length, c.length));
      }
      return false;
  }
  
  function initContactMap() {
    var myLatLng = {lat: 59.3959972, lng: 18.053157};
      var styles = [
        {
            featureType: 'landscape',
            elementType: 'all',
            stylers: [
                { hue: '#383736' },
                { saturation: -93 },
                { lightness: -76 },
                { visibility: 'on' }
            ]
        },{
            featureType: 'water',
            elementType: 'all',
            stylers: [
                { hue: '#dcd5cf' },
                { saturation: -65 },
                { lightness: 32 },
                { visibility: 'on' }
            ]
        },{
            featureType: 'road',
            elementType: 'all',
            stylers: [
                { hue: '#383736' },
                { saturation: -98 },
                { lightness: -66 },
                { visibility: 'on' }
            ]
        },{
            featureType: 'administrative',
            elementType: 'labels',
            stylers: [
                { hue: '#383736' },
                { saturation: 2 },
                { lightness: -58 },
                { visibility: 'off' }
            ]
        },{
            featureType: 'poi',
            elementType: 'all',
            stylers: [
                { hue: '#383736' },
                { saturation: -96 },
                { lightness: -72 },
                { visibility: 'on' }
            ]
        },{
            featureType: 'road',
            elementType: 'labels',
            stylers: [
                { hue: '#777777' },
                { saturation: -100 },
                { lightness: -27 },
                { visibility: 'on' }
            ]
        }
    ];
   
    var options = {
      mapTypeControlOptions: {
        mapTypeIds: [ 'Styled']
      },
      center: myLatLng,
      zoom: 11,
      mapTypeId: 'Styled',
      disableDefaultUI: true
    };
    
    map = new google.maps.Map(document.getElementById('map'), options);
    var styledMapType = new google.maps.StyledMapType(styles, { name: 'Styled' });
    map.mapTypes.set('Styled', styledMapType);
    
    var image = {
      url: Drupal.settings.basePath + 'sites/all/themes/teamolivia/images/big-marker.png',
      size: new google.maps.Size(41, 53),
      origin: new google.maps.Point(0, 0),
      anchor: new google.maps.Point(0, 41)
    };
    
    var shape = {
      coords: [1, 1, 1, 20, 18, 20, 18, 1],
      type: 'poly'
    };
    
    var marker = new google.maps.Marker({
      position: myLatLng,
      map: map,
      icon: image,
      shape: shape,
      title: 'Team Olivia AB'
    });
  }
  
  Drupal.behaviors.initContactPage = {
    attach: function(context, settings) {
      if ($('#block-olivia-common-contact-map').length > 0) {
      google.maps.event.addDomListener(window, 'load', initContactMap);
      google.maps.event.addDomListener(window, "resize", function() {
        var center = map.getCenter();
        google.maps.event.trigger(map, "resize");
        map.setCenter(center); 
      });
      
        
      }
    }
  };

  Drupal.behaviors.initStartpage = {
    attach: function(context, settings) {
      if ($('.panel-col-tree-column').length > 0) {
        $('.panel-col-tree-column .pane-node').matchHeight();
        $('.panel-col-tree-column .pane-block').matchHeight();
        $('.panel-col-tree-column .pane-block .field-name-field-body-text').matchHeight();
      }
      
      if ($('.top-slider').length > 0) { 
        $('.top-slider').cycle({
          swipe: true,
          fx: "scrollHorz",
          timeout: 6000,
          pager: ".pager-wrapper",
          slides: "> .item",
          prev: ".controller.prev",
          next: ".controller.next",
        });
      }
    }
  };
  
  Drupal.behaviors.initAutocomplete = {
    attach: function(context, settings) {
      $("#block-search-form .form-item-search-block-form input").keyup(function() {
        if ($(this).val().length > 0) {
          for (var i = 0; i < ajax_requests.length; i++) {
            ajax_requests[i].abort();
          }
          
          $('.autocomplete-result').remove();
         
          var request = $.getJSON(Drupal.settings.basePath + 'ajax/search/autocomplete/' + $(this).val(),
          {},
          function(data) {
            var html = '<div class="autocomplete-result"><a href="javascript:;" class="close">Stäng</a><ul>';
            var c = 0;
            $.each(data, function(i, item) {
              c++;
              if (c == 1) {
                var c2 = ' class="first"';
              } else {
                var c2 = ' class="not-first"';
              }
              
              html += '<li' + c2 + '><a href="' + item.url + '">' + item.title + '</a></li>';
            });
            
            html += '</ul></div>';
             
            $("#block-search-form .content").after(html);
            
            $('.autocomplete-result .close').on( "click", function() {
              $('.autocomplete-result').remove();
            });
          });
  
          ajax_requests.push(request);
        }
      });
      
      
      $(document).click(function (e) {
        if (!$(e.target).is('#block-search-form *, .search')) {
          $('#block-search-form').removeClass('open');
          $('.autocomplete-result').remove();
        }
        
        if (!$(e.target).is('.region-header .is-expanded > a')) {
          $('.region-header .is-expanded').removeClass('is-open');
        }
        
        if (!$(e.target).is('.person-info, .person-info *, .modal-content *')) {
          $('.modal-content.open').animate({ 
            height: 0
          }, 500, function() {
            $('.modal-content.open').removeClass('open');
            $('.person-info').removeClass('active');
            $('.modal-content').css('height', '');
          });
        }
      });
      
    }
  };
  
  Drupal.behaviors.animateStartpage = {
    // custom function to add animation effects to text in slideshow
    animateHeaderText: function () {
      var $textBox = $('.region-highlighted #block-olivia-common-header-iamge .top-slider .text-box');
      if(!$textBox.length) {
        return;
      }
      var animationName = 'animated fadeIn';
      var animationEnd = 'webkitAnimationEnd mozAnimationEnd MSAnimationEnd oanimationend animationend';
      $textBox.addClass(animationName).one(animationEnd, function() {
        $(this).removeClass('animationName');
      });
    },
    attach: function(context, settings) {
      window.sr = ScrollReveal();

      var revealContent = {
        duration: 800,
        easing: 'ease-in-out'
      };

      sr.reveal('.panel-col-tree-column .entity-bean', revealContent, 50);
      sr.reveal('.panel-col-two-column .view .views-row', revealContent, 50);

      var self = this;
      this.animateHeaderText();
    }
  };
  
  Drupal.behaviors.initOurbusiness = {
    attach: function(context, settings) {
      if ($('.pane-olivia-common-our-business-list').length > 0) {
      
        
        $('.business-wrapper .description > h2').click(function() {
          var $b = $(this).parent().find('.view-list');
          $(this).parent().parent().next('.list-of-companys-container').slideToggle("slow", function() {
            if ($(this).is(':visible')) {
              $b.addClass('open');
              var offset = $('.header').outerHeight();
              
              $('html, body').animate({
                scrollTop: $b.parent().parent().next('.list-of-companys-container').offset().top-offset
              }, 1000);
            } else {
              $b.removeClass('open');
            }
          });
        });
        
        $('.business-wrapper .image img').click(function() {
          var $b = $(this).parent().parent().find('.view-list');
          $(this).parent().parent().next('.list-of-companys-container').slideToggle("slow", function() {
            if ($(this).is(':visible')) {
              $b.addClass('open');
              var offset = $('.header').outerHeight();
              
              $('html, body').animate({
                scrollTop: $b.parent().parent().next('.list-of-companys-container').offset().top-offset
              }, 1000);
            } else {
              $b.removeClass('open');
            }
          });
        });
        
        

        $('.box .view-list').click(function() {
          var $b = $(this);
          $(this).parent().parent().next('.list-of-companys-container').slideToggle("slow", function() {
            if ($(this).is(':visible')) {
              $b.addClass('open');
              var offset = $('.header').outerHeight();
              
              $('html, body').animate({
                scrollTop: $b.parent().parent().next('.list-of-companys-container').offset().top-offset
              }, 1000);
            } else {
              $b.removeClass('open');
            }
          });
        });
        
        $(".country-filter a").click(function() {
          var id = $(this).data('c');
          var offset = $('.header').outerHeight();
          $('html, body').animate({
            scrollTop: $(".c-" + id).offset().top-offset
          }, 1000);
        });
      
        $('.b-items ul li').each(function(index) {
          $(this).find('a.toggle-info-window').qtip({
            suppress: false,
            content: {
              text: $(this).find('.business-window'),
              button: 'Close'
            },
            position: {
              my: 'center',
              at: 'center',
              target: $(window)
            },
            show: 'click',
            hide: 'unfocus',  
          });
          
          $(this).find('a.toggle-info-window').click(function() {
            setTimeout(function(){
              $(window).trigger('resize');
            }, 1500);
          });
        });
      }
    }
  };
  
  Drupal.behaviors.initGlobal = {
    attach: function(context, settings) {
      $('select').selectBox({
        mobile: true,
      });
      
      $('.node-accounting .download-file a').attr('target', '_blank');
      $('.header .search').click(function() {
        $(this).toggleClass('open');
        $('#block-search-form').toggleClass('open');
      });
      
      if ($('body.page-search').length > 0) {
        var title = $('.search-hidden-title').html();
        $('#page-title').html(title);
      }
    }
  };
  
   Drupal.behaviors.initNewsListing = {
    attach: function(context, settings) {
      $('#edit-date-filter-wrapper').change(function() {
        $('#views-exposed-form-news-block-2 .views-submit-button .form-submit').trigger('click');
      });
    }
  };
  

  
  Drupal.behaviors.initHeaderAndNavigation = {
    attach: function(context, settings) {
      $(window).scroll(function() {
        if ($(this).scrollTop() > 1){  
          $('#header, #block-search-form').addClass("sticky");
        } else{
          $('#header, #block-search-form').removeClass("sticky");
        }
      });
      
      $('.region-header .is-expanded > a').click(function(e) {
        e.preventDefault();
        $(this).parent().toggleClass('is-open');
      });
      
      var language_html = '<div class="languages">' + $('#header .languages').html() + '</div>';
      $(language_html).insertAfter(".nav-wrapper .logo");
      
      var menu_html = $('.region-header .block-menu-block').html();
      $('.nav-holder').html(menu_html);
      
      $(window).resize(function() {
        var width = $(window).outerWidth();
        if (width <= 1086) {
          $('.nav-holder .menu-block-wrapper li.expanded').each(function(index) {
            if ($(this).find('ul').length > 0) {
              var mlid = $(this).attr('class').split(' ')[3].split('-')[2];
              if (readCookie('menu_item_' + mlid)) {
                if (readCookie('menu_item_' + mlid) == 'is_closed') {
                  $(this).addClass('submenu-is-open');
                }
              }
            }
          });
        }
      });
      
      var menuStatus;
      
      $(".toggle-menu").click(function(){
        var window_height = $(window).outerHeight();
        var logo = $('.nav-wrapper .logo').outerHeight();
        var offset = 36;
        var new_height = window_height-logo-offset;
        
        if(menuStatus != true){
          $(".wrapper").animate({
            marginLeft: "-335px",
          }, 300, function(){
            menuStatus = true;
            $('.navigation').show();
            $('.nav-holder .menu-block-wrapper').css('height', new_height + 'px');
          });
          
          $(".toggle-menu").addClass('active');
          return false;
        } else {
          
          $('.navigation').hide();
          $(".wrapper").animate({
            marginLeft: "0px",
          }, 300, function(){
            menuStatus = false;
            
          });
          
          $(".toggle-menu").removeClass('active');
          
          return false;
        }
      });
      
      var total_height = $('.wrapper').outerHeight();
      $('.navigation').css('height', total_height + 'px');
      
      
      var $menu_wrapper = $('.navigation');
      
      $('.navigation li.expanded').each(function(index) {
        if ($(this).find('ul').length > 0) {
          $('<span class="sub-menu-button"></span>').insertAfter($(this).find('> a'))
        }
      });
      
      $('.sub-menu-button').click(function() {
        $(this).parent().toggleClass('submenu-is-open');
        var mlid = $(this).parent().attr('class').split(' ')[3].split('-')[2];
        if ($(this).parent().hasClass('submenu-is-open')) {
          createCookie('menu_item_' + mlid, 'is_closed', 30);
        } else {
          createCookie('menu_item_' + mlid, 'is_open', 30);
        }
      });
    }
  };

  Drupal.behaviors.initStaffMemberInfo = {
    attach: function(context, settings) {
      $(window).resize(function() {
        var width = $(window).outerWidth();
        var modalContentWidth = $('#block-olivia-common-staff-members-list').outerWidth();
        var widthOffset = (width-modalContentWidth)/2;
        var style = '';
        style += "#block-olivia-common-staff-members-list .modal-content:after {width: " + widthOffset + "px;right: -" + widthOffset + "px !important;}";
        style += "#block-olivia-common-staff-members-list .modal-content:before {width: " + widthOffset + "px;left: -" + widthOffset + "px !important;}";

        $('.board-listing').html(style);
      });
      
      $(window).trigger('resize');
      
      $('.person-info .title-info .name').matchHeight();
      $('#block-olivia-common-staff-members-list .person-info').click(function() {
        var same = false;
        if ($(this).attr('id') == $('body').data('aperson')) {
          same = true;
        } else {
          $('body').data('aperson', $(this).attr('id'));
          same = false;
        }
        
        var desc = $(this).find('.desc').html();
        var name = $(this).find('.name').html();
        var image = $(this).find('.image img[data-image="large"]').attr('src');
        desc =  '<div class="l"><img src="' + image + '" /></div><div class="r"><h3>' + name + '</h3><div class="desc">' + desc + '</div></div><div class="clear"></div>';
        
        if ($(window).innerWidth() <= 652) {
          var $that = $(this);
          $('.mobile-modal-content').removeClass('open');
          
          if (!$(this).parent('.container').find('.mobile-modal-content').hasClass('open')) {
            $(this).parent('.container').find('.mobile-modal-content').html(desc).addClass('open');
          } else {
            $(this).parent('.container').find('.mobile-modal-content').html(desc).removeClass('open');
          }

          if($(this).parent('.container').prev('.container').find('.mobile-modal-content').hasClass('open')
              || $(this).parent('.container').next('.container').find('.mobile-modal-content').hasClass('open') ) {
            $(this).parent('.container').prev('.container').find('.mobile-modal-content').toggleClass('open');
            $(this).parent('.container').next('.container').find('.mobile-modal-content').toggleClass('open');
          }
          
          $('html, body').animate({
            scrollTop: $that.parent().find('.mobile-modal-content').offset().top-$('.header').outerHeight()
          }, 300);
          
        } else {
          var $that = $(this);
          
          $('.person-info').removeClass('active');
          
          if ($('.modal-content.open').length > 0) {
            $('.modal-content.open').animate({ 
              height: 0
            }, 300, function() {
              $('.modal-content.open').removeClass('open');
              $that.removeClass('active');
              $that.parent().parent().find('.modal-content').css('height', '');
              $('html, body').animate({
                scrollTop: $that.offset().top-$('.header').outerHeight()
              }, 300);
  
              var $elm =  $that.parent().parent().find('.modal-content');
              $elm.html('<div class="i">' + desc + '</div>');
              $elm.addClass('open');
              var $elm_height = $that.parent().parent().find('.modal-content').height();
    
              $elm.css('height', '0px');
              
              if (!same) {
                $that.toggleClass('active');
                
                $elm.animate({ 
                  height: $elm_height
                }, 500);
              } else {
                $('body').data('aperson', 0);
              }
            });
          } else {
            $that.removeClass('active');
            $that.parent().parent().find('.modal-content').css('height', '');
           
            $('html, body').animate({
              scrollTop: $that.offset().top-$('.header').outerHeight()
            }, 300);

            var $elm =  $that.parent().parent().find('.modal-content');
            $elm.html('<div class="i">' + desc + '</div>');
            $elm.addClass('open');
            var $elm_height = $that.parent().parent().find('.modal-content').height();
  
            $elm.css('height', '0px');
            
            $that.toggleClass('active');
            
            $elm.animate({ 
              height: $elm_height
            }, 500);
          }
        }
      });

      if ($('#block-olivia-common-staff-members-list .group .container').length > 0) {
        $('#block-olivia-common-staff-members-list .group .container').matchHeight();
      }

      if ($('.person-info').length > 0) {
        $('.person-info').matchHeight();
      }

      if ($('.person-info .staff-info').length > 0) {
        $('.person-info .staff-info').matchHeight();
      }

      if ($('.person-info .staff-info .contact-info').length > 0) {
        $('.person-info .staff-info .contact-info').matchHeight();
      }
      
      if ($('#block-olivia-common-staff-members-list').length > 0) {
        $('#block-olivia-common-staff-members-list .g').each(function(index) {
          if ($(this).find('.container').length == 1) {
            $(this).addClass('items-one');
          } else if ($(this).find('.container').length == 2) {
            $(this).addClass('items-two');
            $(this).find('.container').addClass('liner');
          } else if ($(this).find('.container').length == 3) {
            $(this).find('.container:eq(1)').addClass('liner');
            $(this).addClass('items-tree');
          }
        });
      }

    }
  };

  Drupal.behaviors.initContactPersonInfo = {
    attach: function(context, settings) {
      if ($('.view-staff.view-display-id-block_1 .views-row').length > 0) {
        $('.view-staff.view-display-id-block_1 .views-row').matchHeight();
      }

      if ($('.view-staff.view-display-id-block_1 .views-row .title-info').length > 0) {
        $('.view-staff.view-display-id-block_1 .views-row .title-info').matchHeight();
      }

      if ($('.pane-address-information .block-boxes').length > 0) {
        $('.pane-address-information .block-boxes').matchHeight();
      }

    }
  };
  
})(jQuery);