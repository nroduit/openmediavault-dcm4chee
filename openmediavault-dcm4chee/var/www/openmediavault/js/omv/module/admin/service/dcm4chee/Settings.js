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
// require("js/omv/WorkspaceManager.js")
// require("js/omv/workspace/form/Panel.js")
// require("js/omv/data/Store.js")
// require("js/omv/data/Model.js")

Ext.define('OMV.module.admin.service.dcm4chee.Settings', {
    extend: 'OMV.workspace.form.Panel',
    uses: [
        'OMV.data.Model',
        'OMV.data.Store'],

    rpcService: 'dcm4chee',
    rpcGetMethod: 'getSettings',
    rpcSetMethod: 'setSettings',

    plugins: [{
        ptype: 'linkedfields',
        correlations: [{
            name: [
                'port']
        }, {
            name: [
                'enable'],
            conditions: [{
                name: 'enable',
                value: true
            }],
            properties: function (valid, field) {
                this.setButtonDisabled('openweb', !valid);
                this.setButtonDisabled('jmx', !valid);
            }
        }]
    }],

    getButtonItems: function () {
        var me = this;
        var items = me.callParent(arguments);
        items.push({
            id: me.getId() + '-update',
            xtype: 'button',
            text: _('Download and install/upgrade dcm4chee'),
            icon: 'images/add.png',
            iconCls: Ext.baseCSSPrefix + 'btn-icon-16x16',
            scope: me,
            handler: Ext.Function.bind(me.onUpdateButton, me, [me])
        });
        return items;
    },

    getFormItems: function () {
        var me = this;
        return [{
            xtype: 'fieldset',
            title: 'General settings',
            defaults: {
                labelSeparator: ''
            },
            items: [{
                xtype: 'checkbox',
                name: 'enable',
                fieldLabel: _('Enable'),
                checked: false
            }, {
                xtype: 'numberfield',
                name: 'port',
                fieldLabel: _('Port'),
                vtype: 'port',
		// TODO change web port in dcm4chee
		disabled: 'true',
                minValue: 1,
                maxValue: 65535,
                allowDecimals: false,
                allowNegative: false,
                allowBlank: false,
                value: 8080,
                plugins: [{
                    ptype: 'fieldinfo',
                    text: _('Port of the PACS web interface.')
                }]
            }]
        }, {
            xtype: 'fieldset',
            title: 'PACS WEB administration',

            defaults: {
                labelSeparator: ''
            },
            items: [{
                id: me.getId() + '-jmx',
                xtype: 'button',
                text: _('JMX Web Interface'),
                icon: 'images/web.png',
                iconCls: Ext.baseCSSPrefix + 'btn-icon-16x16',
                margin: '5 0 10 0',  // Same as CSS ordering (top, right, bottom, left)
                scope: me,
                handler: Ext.Function.bind(me.onOpenJmxButton, me, [me])
            }, {
                id: me.getId() + '-openweb',
                xtype: 'button',
                text: _('PACS Web Interface'),
                icon: 'images/web.png',
                iconCls: Ext.baseCSSPrefix + 'btn-icon-16x16',
                margin: '5 0 10 15',  // Same as CSS ordering (top, right, bottom, left)
                scope: me,
                handler: Ext.Function.bind(me.onOpenPacsButton, me, [me])
            }]
        }];
    },
    onOpenJmxButton: function () {
        var me = this;
        window.open('http://' + window.location.hostname + ':' + me.getForm().findField('port').getValue() + '/admin-dcm4chee', '_blank');
    },

    onOpenPacsButton: function () {
        var me = this;
        window.open('http://' + window.location.hostname + ':' + me.getForm().findField('port').getValue() + '/dcm4chee-web3', '_blank');
    },

    onUpdateButton: function () {
        var me = this;
        var wnd = Ext.create('OMV.window.Execute', {
            title: _('Installing dcm4chee...'),
            rpcService: 'dcm4chee',
            rpcMethod: 'doUpdate',
            rpcParams: {
                uuid: '28b79203-d11b-4cd9-8d1e-5d7803499226'
            },
            hideStartButton: true,
            hideStopButton: true,
            listeners: {
                scope: me,
                finish: function (wnd, response) {
                    wnd.appendValue(_('Done...'));
                    wnd.setButtonDisabled('close', false);
                },
                exception: function (wnd, error) {
                    OMV.MessageBox.error(null, error);
                    wnd.setButtonDisabled('close', false);
                }
            }
        });
        wnd.setButtonDisabled('close', true);
        wnd.show();
        wnd.start();
    }

});

OMV.WorkspaceManager.registerPanel({
    id: 'settings',
    path: '/service/dcm4chee',
    text: _('Settings'),
    position: 10,
    className: 'OMV.module.admin.service.dcm4chee.Settings'
});
