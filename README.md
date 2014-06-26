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

### Usage and Examples

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

#### Arguments

Views Argument Caching with Indexing's `views_arg_cache_index_flush()` function accepts a more structured array of arguments when performing partial argument matching for performing cache flushing. The array of arguments are specified as an two-dimensional associative array keyed by the argument name. Each argument array should contain:

* **value**: The value to perform the operation on.
* **operation**: The operator. Default value is equals (`=`).
* **having**: A boolean value to indicate whether a sub-query should be performed for non-having arguments, matched with this having argument. Default value is a negative boolean (`FALSE`).

```php
$arguments = array(
  'argument_name' => array(
    'value' => 'value-to-match',
  ),
);
```

##### Operators

The operators accepted for performing query matches are the same as the accepted database abstraction layer's conditional operators (e.g. equality `=`). They are:

* Equal (`=`) or Not-equals (`<>`)
* Less-than (`<`) and Less-than or equals to (`<=`)
* Greater-than (`>`) and Greater-than or equals to (`>=`)
* Like (`LIKE`)

When a LIKE operation is performed, the Views Argument Cache with Indexing performs the character escape on the specified `value` property using `db_like()` and concatenates the wildcard characters on the left and right hand side. For example, an argument defined as:

```php
$arguments = array(
  'term_name' => array(
    'value' => 'ABC',
    'operation' => 'LIKE',
  ),
);
```

Would query all `term_name` named arguments whose values contain `ABC`. The `LIKE` operation is useful in situations where more than one value is accepted via a single argument (e.g. a list of taxonomy term identifiers).

### Integration with Rules

Having the ability to flush caches with [Rules](https://drupal.org/project/rules) upon certain actions is very important without the need to write numerous specialized actions for each View cached with the plugin available within. Using the Views Argument Cache with Indexing's integration with the Rules contributed module, a specialized action is provided so that upon the conditions of a Rule, cached views can be cleared that make use of *this* cache plugin.

Arguments specified as part of the action's configuration must adhere to the following format:

```
[argument-name] [operator] [argument-value]
```

Where `[argument-name]` represents the name of the view's contextual argument in which the `[argument-value]` is used in conjunction to the `[argument-name]` for generating the cached view's identifier. Similarly to performing the cache flush programmatically with `views_arg_cache_index_flush()`, the `[operator]` must be specified as well. Using the previously started example above, the same argument specification can be achieved in the Rules action configuration with:

```
term_name LIKE abc
```

**Note:** Each separate argument must be specified on its own new line.

### License

This Views Argument Cache with Indexing is licensed under the [GNU General Public License](./LICENSE.md) version 2.
