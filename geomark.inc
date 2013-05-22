<?php
/**
 * @file geomark.inc
 *
 * Entry point for php geomark api
 */
include_once('lib/geomark.read.inc');

class geomark {
  public static function loadReader($id) {
    $geomark_reader = new geomark_reader($id);

    return $geomark_reader;
  }

  public static function getBoundingBox($id, $file_type = 'wkt', $srid = 4326) {
    $gm = new geomark_reader($id);
    $gm->open('boundingBox', $file_type, $srid);

    $geodata = '';
    while ($data = $gm->getPart()) {
      $geodata .= $data;
    }

    return $geodata;
  }

  public static function getFeature($id, $file_type = 'wkt', $srid = 4326) {
    $gm = new geomark_reader($id);
    $gm->open('feature', $file_type, $srid);

    $geodata = '';
    while ($data = $gm->getPart()) {
      $geodata .= $data;
    }

    return $geodata;
  }

  public static function getInfo($id, $file_type = 'json') {
    $gm = new geomark_reader($id);
    $gm->open('info', $file_type);

    $geodata = '';
    while ($data = $gm->getPart()) {
      $geodata .= $data;
    }

    return $geodata;
  }

  /**
   * Returns an array of the lines returned from the geomark REST query.
   *
   * @param $output if set to 'array' an array of parts is returned.  If set to 'text'
   *                a block of text is returned.
   */
  public static function getParts($id, $file_type = 'wkt', $srid = 4326, $output = 'array') {
    $gm = new geomark_reader($id);
    $gm->open('parts', $file_type, $srid);

    switch ($output) {
      case 'array':
        $geodata = array();
        while ($data = $gm->getPart()) {
          $geodata[] = $data;
        }
        break;
      case 'text':
        $geodata = '';
        while ($data = $gm->getPart()) {
          $geodata .= $data;
        }
        break;
    }

    if (!in_array($output, array('text', 'array'))) {
      // Could throw an error here.
      return;
    }
    return $geodata;
  }

  public static function getPoint($id, $file_type = 'wkt', $srid = 4326) {
    $gm = new geomark_reader($id);
    $gm->open('point', $file_type, $srid);

    $geodata = '';
    while ($data = $gm->getPart()) {
      $geodata .= $data;
    }

    return $geodata;
  }

  /**
   * Get an array of valid file types and the actions they apply to.
   */
  public static function fileTypes() {
    return array(
      'json' => array('info'),
      'xml' => array('info'),
      'kml' => array('read', 'write'),
      'shpz' => array('read', 'write'),
      'shp' => array('read', 'write'),
      'geojson' => array('read', 'write'),
      'gml' => array('read', 'write'),
      'wkt' => array('read', 'write'),
    );
  }

  /**
   * Check if geoPHP is included.
   *
   * @todo add optional geoPHP integration.
   */
  public static function geoPHPIncluded($force = NULL) {
    static $geoPHP_included = NULL;
    if ($force !== NULL) $geoPHP_included = $force;
    if ($geoPHP_included !== NULL) {
      return $geoPHP_included;
    }
    $geoPHP_included = class_exists('geoPHP');
    return $geoPHP_included;
  }
}