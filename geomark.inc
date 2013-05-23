<?php
/**
 * @file geomark.inc
 *
 * Entry point for php geomark api
 */
include_once('lib/geomark.read.inc');
include_once('lib/geomark.write.inc');

class geomark {
  public static function loadReader($id) {
    $geomark_reader = new geomark_reader($id);
    return $geomark_reader;
  }

  public static function getBoundingBox($id, $file_type = 'wkt', $srid = 4326) {
    $gm = new geomark_reader($id);
    $gm->open('boundingBox', $file_type, $srid);

    return $gm->getString();
  }

  public static function getFeature($id, $file_type = 'wkt', $srid = 4326) {
    $gm = new geomark_reader($id);
    $gm->open('feature', $file_type, $srid);

    return $gm->getAllString();
  }

  public static function getInfo($id, $file_type = 'json') {
    $gm = new geomark_reader($id);
    $gm->open('info', $file_type);

    return $gm->getAllString();
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
        $geodata = $gm->getAllArray();
        break;
      case 'text':
        $geodata = $gm->getAllString();
        break;
    }

    if (!in_array($output, array('text', 'array'))) {
      // Could throw an error here.
      return FALSE;
    }

    return $geodata;
  }

  public static function getPoint($id, $file_type = 'wkt', $srid = 4326) {
    $gm = new geomark_reader($id);
    $gm->open('point', $file_type, $srid);

    return $gm->getAllString();
  }

  /**
   * Get an array of valid file types and the actions they apply to.
   */
  public static function fileTypes() {
    return array(
      'json' => array('mime' => 'application/json', 'info'),
      'xml' => array('mime' => 'text/xml', 'info'),
      'kml' => array('mime' => 'application/vnd.google-earth.kml+xml', 'read', 'write'),
      'shpz' => array('mime' => 'application/x-shp+zip', 'read', 'write'),
      'shp' => array('mime' => 'application/x-shp', 'read', 'write'),
      'geojson' => array('mime' => 'application/x-geo+json', 'read', 'write'),
      'gml' => array('mime' => 'application/gml+xml', 'read', 'write'),
      'wkt' => array('mime' => 'text/plain', 'read', 'write'),
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