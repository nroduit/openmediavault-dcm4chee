<?php
require_once("openmediavault/functions.inc");
\OMV\System\LogFileSpec::registerSpecification("dcm4chee", [
  "filename" => "server.log",
  "filepath" => "/var/lib/dcm4chee/server/default/log/server.log",
  "regex" => "/^(\S+\s+\S+)\s+(.*)$/",
  "columns" => [
      "ts" => 1,
      "date" => 1,
      "message" => 2
  ]]);

