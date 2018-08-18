<?php
/**
 * Copyright (C) 2014-2015 OpenMediaVault Plugin Developers
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

namespace OMV\Engined\Rpc;

use DateTime;
use Exception;
use OMV\Config\Database;
use OMV\Rpc\ServiceAbstract;
use OMV\Rpc\ServiceManager as RpcServiceManager;
use OMV\System\Process;

class OMVRpcServiceDcm4chee extends ServiceAbstract
{

	/** @var Database */
	private $database;
	/** @var string */
  private $dataModelPath = 'conf.service.dcm4chee';

	public function __construct(Database $database = null)
	{
		$this->database = $database ?: Database::getInstance();
	}

    public function getName()
    {
        return 'dcm4chee';
    }

    public function initialize()
    {
        $this->registerMethod('getSettings');
        $this->registerMethod('setSettings');
        $this->registerMethod('doRestart');
        $this->registerMethod('doUpdate');
    }

    public function getSettings($params, $context)
    {
				$this->validateMethodContext($context, ['role' => OMV_ROLE_ADMINISTRATOR]);

        // Get configuration object
   			$object = $this->database->get($this->dataModelPath);

        // Modify result data
        // $object['enable']  = boolval($object['enable']);
        // $object['port']    = 8080;
        //$object['port']    = intval($object['port']);
        // $object['showtab'] = boolval($object['showtab']);

        return $object->getAssoc();
    }

    public function setSettings($params, $context)
    {
			$this->validateMethodContext($context, ['role' => OMV_ROLE_ADMINISTRATOR]);
			$this->validateMethodParams($params, 'rpc.dcm4chee.setsettings');

			$object = $this->database->get($this->dataModelPath);
			$object->setAssoc($params);

			$this->database->set($object);

			return $object->getAssoc();
    }

    public function doRestart($params, $context)
    {
        // Validate the RPC caller context.
				$this->validateMethodContext($context, ['role' => OMV_ROLE_ADMINISTRATOR]);

        $object = $this->database->get($this->dataModelPath);

        if (!boolval($object['enable']))
        {
            return;
        }

		// $systemCtl = new SystemCtl($this->serviceName);
		// $systemCtl->enable(true);

        $cmd = new Process('omv-mkconf', 'dcm4chee restart');
        $cmd->setRedirect2to1();
        $cmd->execute();
    }

    public function doUpdate($params, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ['role' => OMV_ROLE_ADMINISTRATOR]);

        $this->validateMethodParams($params, "rpc.common.objectuuid");

        // Create a background process.
		return $this->execBgProc(function($bgStatusFilename, $bgOutputFilename)
		  use ($params) {
			$cmdArgs = 'dcm4chee update';
			$cmd = new Process('omv-mkconf', $cmdArgs);
			$cmd->setRedirect2to1();
			// Execute the command.
			$exitStatus = $this->exec($cmd->getCommandLine(), $output,
				$bgOutputFilename);
			// Check the exit status:
			// Bit 0: Command line did not parse
			// Bit 1: Device open failed, or device did not return an
			//        IDENTIFY DEVICE structure
			if (($exitStatus !== 0) && (($exitStatus & 0) ||
				($exitStatus & 1))) {
				throw new \OMV\ExecException($cmd->getCommandLine(), $output,
					$exitStatus);
			}
			return $output;
		});
    }
}
