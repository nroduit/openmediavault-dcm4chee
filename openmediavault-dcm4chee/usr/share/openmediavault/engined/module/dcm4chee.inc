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
 namespace OMV\Engined\Module;

 use OMV\Config\Database;
 use OMV\Engine\Module\IServiceStatus;
 use OMV\Engine\Module\Manager as ModuleManager;
 use OMV\Engine\Module\ServiceAbstract;
 use OMV\Engine\Notify\Dispatcher;
 use OMV\Engine\Notify\IListener;
 use OMV\System\Process;
 use OMV\System\SystemCtl;

class OMVModuleDcm4chee extends ServiceAbstract implements
    IListener,
    IServiceStatus
{
  /** @var Database */
  private $database;

  /** @var string */
  private $dataModelPath = 'conf.service.dcm4chee';

  /** @var string */
  private $eventMessagePath = 'org.openmediavault.conf.service.dcm4chee';

  /** @var ModuleManager */
  private $moduleManager;

  /** @var string */
  private $serviceName = 'dcm4chee.service';

  public function __construct(Database $database = null)
  {
      $this->database = $database ?: Database::getInstance();
  }

    public function getName()
    {
        return 'dcm4chee';
    }
    public function getStatus()
    {
      $object = $this->database->get($this->dataModelPath);
      $systemCtl = new SystemCtl($this->serviceName);

      return [
          'name' => $this->getName(),
          'title' => gettext('PACS'),
          'enabled' => $object->get('enable'),
          'running' => $systemCtl->isActive(),
      ];
    }

    public function applyConfig()
    {
      $cmd = new Process('omv-mkconf', $this->getName());
      $cmd->setRedirect2to1();
      $cmd->execute();
    }

    public function startService()
    {
      $object = $this->database->get($this->dataModelPath);

      if ($object->get('enable')) {
          $systemCtl = new SystemCtl($this->serviceName);
          $systemCtl->enable(true);
      }
    }

    public function stopService()
    {
      $systemCtl = new SystemCtl($this->serviceName);
      $systemCtl->disable(true);
    }

    public function getNotificationConfig() {
  		return [
  			[
  				'id' => $this->getName(),
  				'type' => getText('Services'),
  				'title' => gettext('PACS')
  			]
  		];
  	}

    public function bindListeners(Dispatcher $dispatcher)
    {
        $dispatcher->addListener(
            OMV_NOTIFY_CREATE | OMV_NOTIFY_MODIFY | OMV_NOTIFY_DELETE,
            $this->eventMessagePath,
            [$this, 'setDirty']
        );
    }
}
