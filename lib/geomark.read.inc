<?php
/**
 * @file geomark.read.inc
 *
 * Code related to reading and retrieving information about existing Geomarks
 */
include_once('geomark.write.inc');

class geomark_reader {
  var $id;
  private $handler;
  private $base_url = 'http://apps.gov.bc.ca/pub/geomark/geomarks/gm-';

  public function __construct($id) {
    if (strpos($id, $this->base_url) !== FALSE) {
      $id = str_replace($this->base_url, '', $id);
    }
    $this->id = $id;
  }

  public function setUrl($url = 'http://apps.gov.bc.ca/pub/geomark/geomarks/gm-') {
    $this->base_url = $url;
  }

  public function getUrl() {
    return $this->base_url;
  }

  /**
   * Open a file streamer to geomark for the purpose of reading in information
   * about an existing geomark.
   */
  public function open() {
    $args = func_get_args();
    $query_type = array_shift($args);
    $file_type = array_shift($args);
    $srid = array_shift($args);

    if ($query_type == 'info') {
      $url = $this->base_url . $this->id . '.' . $file_type;
    } else {
      $url = $this->base_url . $this->id . '/' . $query_type . '.' . $file_type;

      if ($srid !== NULL) {
        $url .= '?srid=' . $srid;
      }
    }

    $this->handler = fopen($url, 'r');
  }

  /**
   * Wrapper for fgets on the open file stream
   */
  public function getPart() {
    return fgets($this->handler);
  }

  public function getAllArray() {
    $geodata = array();
    while ($data = $this->getPart()) {
      $geodata[] = $data;
    }

    return $geodata;
  }

  public function getAllString() {
    $geodata = '';
    while ($data = $this->getPart()) {
      $geodata .= $data;
    }

    return $geodata;
  }

  public function close() {
    fclose($this->handler);
  }

  /**
   * Destructor for geomark object.
   */
  function __destruct() {
    @fclose($this->handler);
    unset($this->id);
  }
}