<?php
/**
 * @author    Volker Theile <volker.theile@openmediavault.org>
 * @copyright Copyright (C) 2009-2014 Volker Theile
 * @copyright Copyright (C) 2014 OpenMediaVault Plugin Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
require_once("openmediavault/module.inc");
require_once("openmediavault/config.inc");
require_once("openmediavault/error.inc");
require_once("openmediavault/util.inc");
require_once("openmediavault/initscript.inc");
require_once("openmediavault/functions.inc");
require_once("openmediavault/log.inc");

class OMVModuleDcm4chee extends OMVModuleServiceAbstract implements
    OMVINotifyListener,
    OMVIModuleServiceStatus
{

    public function getName()
    {
        return "dcm4chee";
    }

    private function getPluginName()
    {
        return strtolower( $this->getName() );
    }

    private function getXPath()
    {
        return sprintf( "//services/%s", $this->getPluginName() );
    }

    private function getEventMessagePath()
    {
        return sprintf( "org.openmediavault.services.%s", $this->getPluginName() );
    }

    private function getSharedFolderEventMessagePath()
    {
        return "org.openmediavault.system.shares.sharedfolder";
    }

    public function getStatus()
    {
        global $xmlConfig;
        $object = $xmlConfig->get($this->getXpath());
        if (is_null($object)) {
            throw new OMVException(
                OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,
                $this->getXpath()
            );
        }
        $initd = new OMVSysVInitScript($this->getPluginName());
        return array(
            "name" => $this->getPluginName(),
            "title" => gettext("PACS"),
            "enabled" => boolval($object["enable"]),
            "running" => $initd->status() == 0
        );
    }

    public function applyConfig()
    {
        $cmd = sprintf("omv-mkconf %s config 2>&1", $this->getPluginName());

        if (0 !== $this->exec($cmd, $output)) {
            throw new OMVException(
                OMVErrorMsg::E_EXEC_FAILED,
                $cmd,
                implode("\n", $output)
            );
        }
    }

    public function startService()
    {
        global $xmlConfig;
        $object = $xmlConfig->get( $this->getXPath() );

        if (is_null($object)) {
            throw new OMVException(
                OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,
                $this->getXPath()
            );
        }

        if (FALSE === boolvalEx($object['enable']))
            return;

        $initd = new OMVSysVInitScript($this->getPluginName(), TRUE);
        $initd->enable();

        switch($initd->status()) {
        case 0: // running -> restart
            $initd->restart();
            break;
        default: // start service
            $initd->start();
        }            
    }

    public function stopService()
    {
        $initd = new OMVSysVInitScript($this->getPluginName(), FALSE);
        $initd->exec();        
    }

    public function bindListeners(OMVNotifyDispatcher $dispatcher)
    {
        $dispatcher->addListener(
            OMV_NOTIFY_CREATE | OMV_NOTIFY_MODIFY | OMV_NOTIFY_DELETE,
            $this->getEventMessagePath(),
            array($this, "setDirty")
        );

    }
}

// Register module
$moduleMgr = &OMVModuleMgr::getInstance();
$moduleMgr->registerModule(new OMVModuleDcm4chee());

// Register new logfile type.
OMVLogFile::registerType("dcm4chee", array(
  "filename" => "server.log",
  "filepath" => "var/lib/dcm4chee/server/default/log/server.log",
  "regex" => "/^(\S+\s+\S+)\s+(.*)$/",
  "columns" => array(
      "ts" => 1,
      "date" => 1,
      "message" => 2
  )));

