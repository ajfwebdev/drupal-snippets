<div class="teasers__collection">
  <?php foreach ($items as $item): ?>
  <div class="teasers" <?php print $item['background'];?>>
    <!-- if post has featured image use as background -->
    <div class="teasers__author">
      <img class="author-image" src="<?php print $item['author_image']; ?>" alt="<?php print $item['author_name'];?>" />
      <span>By <?php print $item['author_name']; ?></span>
    </div>
    <h2 class="teasers__title"><?php print $item['title']; ?></h2>
    <button class="teasers__cta btn read-more">
      <?php print $item['link']; ?>
    </button>
  </div>
  <?php endforeach; ?>
</div>
