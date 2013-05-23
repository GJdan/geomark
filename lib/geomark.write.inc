<?php
/**
 * @file geomark.write.inc
 *
 * Code for creating new Geomarks
 */
include_once('geomark.read.inc');

class geomark_writer {
  private $base_url = 'http://apps.gov.bc.ca/pub/geomark/geomarks/';
  private $params;

  public function __construct($format, $data, $srid = 4326) {
    $this->params = array(
      'format' => $format,
      'multiple' => TRUE,
      'body' => $data,
      'srid' => $srid,
    );
  }

  public function setGeometryType($geometry_type) {
    $this->params['geometryType'] = $geometry_type;
  }

  public function setMultiple($multiple) {
    $this->params['multiple'] = $multiple;
  }

  public function setBuffer($buffer, $join = 'ROUND', $cap = 'ROUND', $limit = 5, $segments = 8) {
    $this->params['bufferMetres'] = $buffer;
    $this->params['bufferJoin'] = $join;
    $this->params['bufferCap'] = $cap;
    $this->params['bufferMitreLimit'] = $limit;
    $this->params['bufferSegments'] = $segments;
  }

  /**
   * Create the geomark represented by this object.
   *
   * Builds an HTTP POST request and sends it with information
   * needed to create a new Geomark via the Geomark REST API.
   *
   * @return a geomark_reader object on success, or the http status code on failure.
   */
  public function create() {
    foreach ($this->params as $key => &$value) {
      if (is_array($value)) {
        $value = implode(',', $value);
      }

      $post_params[] = $key.'='.urlencode($value);
      //$post_params[] = $key . '=' . $value;
    }
    $post_string = implode('&', $post_params);

    $parts = parse_url($this->base_url . 'new');

    $fp = fsockopen($parts['host'],
          isset($parts['port'])?$parts['port']:80,
          $errno, $errstr, 30);

    $out = "POST ".$parts['path']." HTTP/1.1\r\n";
    $out.= "Host: ".$parts['host']."\r\n";
    $out.= "Content-Type: application/x-www-form-urlencoded\r\n";
    $out.= "Content-Length: ".strlen($post_string)."\r\n";
    $out.= "Connection: Close\r\n\r\n";
    if (isset($post_string)) {
      $out.= $post_string;
    }

    fwrite($fp, $out);

    $response = '';
    while(!feof($fp)) {
      $response .= fgets($fp, 8192);
    }
    fclose($fp);

    return $this->evaluateResponse($response);
  }

  private function evaluateResponse($response) {
    $response_lines = explode("\n", $response);

    $results = array();
    $status_code = '';
    foreach ($response_lines as $line) {
      $line = trim($line);
      if ($line) {
        $line_array = explode(': ', $line);
        if (count($line_array) < 2 && strpos($line, 'HTTP/1.1') !== FALSE) {
          $status_code = $line;
        } else {
          $results[$line_array[0]] = $line_array[1];
        }
      }
    }

    // If the status code doesn't show that a geomark was successfully created then return the status code.
    if (strpos($status_code, 'HTTP/1.1 302') === FALSE) {
      return $status_code;
    }

    $geomark = geomark::loadReader($results['Location']);
    return $geomark;
  }
}