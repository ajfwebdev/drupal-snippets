<div class="my-blog__collection">
  <?php foreach ($items as $item): ?>
  <div class="my-blog" <?php print $item['background'];?>>
    <!-- if post has featured image use as background -->
    <div class="my-blog__author">
      <img class="author-image" src="<?php print $item['author_image']; ?>" alt="<?php print $item['author_name'];?>" />
      <span>By <?php print $item['author_name']; ?></span>
    </div>
    <h2 class="my-blog__title"><?php print $item['title']; ?></h2>
    <button class="my-blog__cta btn read-more">
      <?php print $item['link']; ?>
    </button>
  </div>
  <?php endforeach; ?>
</div>
