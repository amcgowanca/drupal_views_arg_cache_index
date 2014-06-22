## Views argument cache with indexing

Views Argument Cache with Indexing is a Drupal 7 module derived from [Views Arg Cache](http://drupal.org/project/views_arg_cache). Views Argument Cache with Indexing performs additional database storage of the cached view's arguments and there values which allows for additional filtering when performing a cache flush via the use of `views_arg_cache_index_flush`.

This module attempts to address one of the core problems of the [Views Arg Cache](http://drupal.org/project/views_arg_cache) contributed module. The problem is simply that developers are unable to trigger a cache flush for a particular cached view if the arguments of the view are only partially known.

### Installation

#### Common Installation

* Download the module and place it within the `sites/all/modules` directory under the directory named `views_arg_cache_index`.
* Install the module either via the Administrative interface or by using Drush, `drush en views_arg_cache_index --yes`.

#### As part of Drush Make

This project can be downloaded and used within a Drush make file like any other contributed module. The difference is that the module exists on GitHub instead of Drupal.org. Within your `drupal-org.make`, the Views Argument Cache with Indexing module can be defined as a project. This is done with:

```
projects[views_arg_cache_index][type] = "module"
projects[views_arg_cache_index][download][type] = "git"
projects[views_arg_cache_index][download][url] = "git@github.com:amcgowanca/drush_views_arg_cache_index.git"
projects[views_arg_cache_index][download][tag] = "7.x-0.1"
```

**Notes:**

* This module is downloaded using Git.
* This module will be downloaded as the tagged release version 7.x-0.1.

### Usage

Similar to the [Views Arg Cache](http://drupal.org/project/views_arg_cache) contributed module, the Views Argument Cache with Indexing only flushes its cache objects with a Clear All Caches invocation (e.g. `drush cc all`) *OR* with the implementation of custom code cache flushing.

#### Flush a view cache with all arguments

Most of the time, all arguments of a view that is to have its cache flushed are known. As a result, a developer may clear the cached objects of a view when all arguments are known by specifying the fourth parameter of `views_arg_cache_index_flush` named `$args_match_full` as a boolean `TRUE`. For example,

```php
views_arg_cache_index_flush('my_custom_view', 'view_display_id', $arguments, TRUE);
```

#### Flush a view cache with partial arguments match

When a view's cache objects need to be flushed and only *some* of the arguments are known, the Views Argument Cache with Indexing module differentiates itself. Views Argument Cache with Indexing implements secondary storage tables in which the arguments of the cached view object are stored in association with the cache object's identifier allowing for easy partial match flushing to occur.

For example, perhaps you have a defined view that has two contextual arguments:

* An organic group identifier, represented by `gid`.
* A node identifier, represented by `nid`.

When this view is rendered, the results and output of the view are cached with a unique cache identifier represented in the following string format:

```
view_name:display_id:md5(serialized-array-of-arguments)
```

As a result, it is virtually impossible to clear the cache objects for all cached views who has the argument organic group identifier argument equalling one (`$gid = 1;`). Views Argument Cache with Indexing allows for such partial argument matching to occur. An example to clear all view cache objects with the organic group identifier value of one would be:

```php
$arguments = array(
  'gid' => array(
    'value' => 1,
    'operation' => '='
  )
);

views_arg_cache_index_flush('view_name', 'display_name', $arguments);
```

### License

This Views Argument Cache with Indexing is licensed under the [GNU General Public License](./LICENSE.md) version 2.
