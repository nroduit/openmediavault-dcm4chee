/**
 * @license   http://www.gnu.org/licenses/gpl.html GPL Version 3
 * @author    Nicolas Roduit<nicolas.roduit@gmail.com>
 * @copyright Copyright (c) 2013-2014 OpenMediaVault Plugin Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// Register a node in the navigation tree.
//
// id: 
//     Set the ID of the node.
// path: 
//     Parent path in the navigation view.
// Text: 
//     Service name/title. This is displayed in the navigation.
// icon16: 
//     16x16 pixel icon that is displayed in the navigation tree.
// iconSvg: 
//     SVG icon that is displayed in the navigation view.
OMV.WorkspaceManager.registerNode({
    id: "dcm4chee",
    path: "/service",
    text: "PACS", 
    icon16: "images/dcm4chee.png",
    iconSvg: "images/dcm4chee.svg"
});
