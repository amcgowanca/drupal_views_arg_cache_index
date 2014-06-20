<?php
/**
 * @file
 * plugins/views_arg_cache_index_plugin_cache.inc
 */

/**
 * Views cache plugin for caching views by arguments.
 */
class views_arg_cache_index_plugin_cache extends views_plugin_cache {
  public $table = VIEWS_ARG_INDEX_CACHE_TABLE;

  public function get_results_key() {
    if (!isset($this->_results_key)) {
      $args = $this->truncate_args($this->view->args);
      $this->_results_key = _views_arg_cache_index_create_key($this->view->name, $this->display->id, $args) . ':results:' . $this->get_cache_key();
    }
    return $this->_results_key;
  }

  public function get_output_key() {
    if (!isset($this->_output_key)) {
      $key_data = array(
        'theme' => $GLOBALS['theme'],
      );
      $args = $this->truncate_args($this->view->args);
      $this->_output_key = _views_arg_cache_index_create_key($this->view->name, $this->display->id, $args) . ':output:' . $this->get_cache_key($key_data);
    }
    return $this->_output_key;
  }

  public function cache_set($type) {
    parent::cache_set($type);

    switch ($type) {
      case 'results':
        $cid = $this->get_results_key();
        break;
      case 'output':
        $cid = $this->get_output_key();
        break;
      default:
        return;
    }

    $exists = db_select('views_arg_cache_index', 't')
      ->fields('t', array('id'))
      ->condition('t.cid', $cid)
      ->execute()
      ->fetchAssoc();
    if (!$exists) {
      $id = db_insert('views_arg_cache_index')
        ->fields(array(
          'cid' => $cid,
          'view' => $this->view->name,
          'display' => $this->display->id,
          'type' => $type,
        ))
        ->execute();

      $i = 0;
      foreach ($this->view->argument as $arg_name => $argument) {
        $arg_value = isset($this->view->args[$i]) ? $this->view->args[$i] : NULL;
        db_insert('views_arg_cache_index_arguments')
          ->fields(array(
            'index_id' => $id,
            'arg_name' => $arg_name,
            'arg_value' => $arg_value,
          ))
          ->execute();
        $i++;
      }
    }
  }

  protected function truncate_args(array $args = array()) {
    if ($this->options['arg_count']) {
      return array_slice($args, 0, $this->options['arg_count']);
    }
    return $args;
  }

  public function summary_title() {
    return t('Argument-based with index');
  }

  public function option_definition() {
    $options = parent::option_definition();
    $options['arg_count'] = array(
      'default' => 0,
    );
    return $options;
  }

  public function option_defaults(&$options) {
    $options['arg_count'] = 0;
  }

  public function options_form(&$form, &$form_state) {
    $options = array(0 => t('All'));

    $args_count = count($this->view->argument);
    if (1 <= $args_count) {
      for ($i = 1; $i <= $args_count; $i++) {
        $options[$i] = $i;
      }
    }

    $form['arg_count'] = array(
      '#type' => 'select',
      '#title' => t('Number of arguments to use'),
      '#description' => t('Select the amount of arguments to generate the key. The arguments will be used in order of which they appear. This is useful if not all arguments are significant for caching.'),
      '#options' => $options,
      '#default_value' => $this->options['arg_count'],
    );
  }
}
