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

### License

This Views Argument Cache with Indexing is licensed under the [GNU General Public License](./LICENSE.md) version 2.
