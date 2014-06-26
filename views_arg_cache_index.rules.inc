<?php
/**
 * @file
 * views_arg_cache_index.rules.inc
 */

/**
 * Implements hook_rules_action_info().
 */
function views_arg_cache_index_rules_action_info() {
  return array(
    'views_arg_cache_index_cache_flush' => array(
      'group' => t('Views Arg Cache Index'),
      'label' => t('Clear the cache for views by arguments'),
      'base' => 'views_arg_cache_index_rules_cache_flush',
      'parameter' => array(
        'view' => array(
          'type' => 'text',
          'label' => t('Views and displays'),
          'description' => t("The view's name and display to clear the cache for."),
          'options list' => '_views_arg_cache_index_rules_get_views_list',
          'save' => TRUE,
          'restriction' => 'input',
        ),
        'arguments' => array(
          'type' => 'text',
          'label' => t('Arguments of cached view'),
          'description' => t('Each argument should be specified on its own line. Arguments listed should be structured with the "[argument-name] [operator] [argument-value]" OR "[argument-name] LIKE [argument-value]".'),
          'optional' => TRUE,
        ),
      ),
    ),
  );
}

/**
 * Executes the `view_arg_cache_index_cache_flush` rules action.
 *
 * @param string $view_info
 *   The view's name and display id to clear ([view-name]:[display-id]).
 * @param string $arguments
 */
function views_arg_cache_index_rules_cache_flush($view_info, $arguments) {
  list($view_name, $display_id) = explode(':', $view_info);
  $arguments = _views_arg_cache_index_rules_process_arguments($arguments);
  views_arg_cache_index_flush($view_name, $display_id, $arguments);
}

/**
 * Callback for building select list of view definitions.
 *
 * @return array
 *   Returns the option list array.
 */
function _views_arg_cache_index_rules_get_views_list() {
  $options = array();
  foreach (views_get_enabled_views() as $view) {
    foreach ($view->display as $display) {
      if (!empty($display->display_options['cache']) && 'views_arg_cache_index' == $display->display_options['cache']['type']) {
        $options[$view->human_name][$view->name . ':' . $display->id] = $view->human_name . ': ' . (!empty($display->title) ? $display->title : $display->id);
      }
    }
  }
  return $options;
}

/**
 * Processes the Rule's Action specified arguments into a normalized array.
 *
 * @param string $arguments
 *   The non-processed string of argument data to process.
 *
 * @return array
 *   Returns the processed array of arguments.
 *
 * @throws Exception
 *   Thrown if the argument appears to be malformed.
 */
function _views_arg_cache_index_rules_process_arguments($arguments) {
  $processed = array();
  $arguments = trim($arguments);
  if (!empty($arguments)) {
    $arguments = explode("\n", $arguments);
    foreach ($arguments as $argument) {
      $argument = explode(' ', $argument);
      if (3 !== count($argument)) {
        throw new Exception(t('Malformed argument value Views Arguments Cache with Indexing rule.'));
      }

      $argument = array_map('trim', $argument);
      $argument = array_map('check_plain', $argument);

      list($arg_name, $operator, $value) = $argument;

      $processed[$arg_name] = array(
        'value' => $value,
        'operation' => $operator,
      );
    }
  }
  return $processed;
}
