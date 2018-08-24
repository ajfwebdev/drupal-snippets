/**
 * @file
 * Javascript for a content type edit form page.
 * It checks if a video was selected in a Media field, and hides some
 * other fields if the file type is video.
 * PHP logic determines what type of file it is given the file ID.
 */

(function ($) {

Drupal.behaviors.myContentTypeEditForm = {
  attach: function (context) {

    var fid = $('.media-item', context).attr("data-fid");

    // Use custom endpoint that can detect file type by id.
    var url = Drupal.settings.basePath + 'my-path/ajax/file-type/' + fid;

    $.ajax({
      url: url,
      cache: false,
      success: function (result) {
        var return_json = JSON.parse(result);
        if (return_json.success === 'true' && typeof return_json.data !== null && return_json.data === 'video') {
          $('.group-author').hide();
          $('#edit-body').hide();
        }
        else {
          $('.group-author').show();
          $('#edit-body').show();
        }
      },
      error: function (e) {
      },
    });
  }
};

})(jQuery);
