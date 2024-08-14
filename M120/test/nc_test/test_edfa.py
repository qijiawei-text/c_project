import unittest
import nctest

class TestEDFA(nctest.NCTestCase):
    """ Tests basic NETCONF operations on the device YANG module. """
    def __init__(self, *args, **kwargs):
        super(TestEDFA, self).__init__(*args, **kwargs)
        self.ns = {"nc": "urn:ietf:params:xml:ns:netconf:base:1.0", "dev": "http://org/openroadm/device"}

    # check edfa
    def test_edfa(self):
        filter_xml = """<nc:filter xmlns:nc="urn:ietf:params:xml:ns:netconf:base:1.0">
            <open-optical-device xmlns="http://org/openroadm/device">
                <optical-amplifier/>
            </open-optical-device>
            </nc:filter>"""
        reply = self.nc.get_config(source="running", filter=filter_xml)
        self.check_reply_data(reply)
        #check amplifier
        amplifiers = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier", namespaces=self.ns)
        self.assertNotEqual(len(amplifiers), 0)
        for amplifier in amplifiers:
            self.assertTrue(amplifier.tag.endswith("amplifier"))

    # check edfa config
    def test_edfa_config(self):
        filter_xml = """<nc:filter xmlns:nc="urn:ietf:params:xml:ns:netconf:base:1.0">
            <open-optical-device xmlns="http://org/openroadm/device">
                <optical-amplifier>
                    <amplifiers>
                       <amplifier/>
                    </amplifiers>
                </optical-amplifier>
            </open-optical-device>
            </nc:filter>"""
        reply = self.nc.get_config(source="running", filter=filter_xml)
        self.check_reply_data(reply)
        #check name
        names = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier/dev:config/dev:name", namespaces=self.ns)
        self.assertNotEqual(len(names), 0)
        for name in names:
            self.assertTrue(name.tag.endswith("name"))
        #check type
        types = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier/dev:config/dev:type", namespaces=self.ns)
        self.assertNotEqual(len(types), 0)
        for type in types:
            self.assertTrue(type.tag.endswith("type"))
        #check target-gain
        target_gains = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier/dev:config/dev:target-gain", namespaces=self.ns)
        self.assertNotEqual(len(target_gains), 0)
        for target_gain in target_gains:
            self.assertTrue(target_gain.tag.endswith("target-gain"))
        #check target-gain-tilt
        target_gain_tilts = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier/dev:config/dev:target-gain-tilt", namespaces=self.ns)
        self.assertNotEqual(len(target_gain_tilts), 0)
        for target_gain_tilt in target_gain_tilts:
            self.assertTrue(target_gain_tilt.tag.endswith("target-gain-tilt"))
        #check gain-range
        gain_ranges = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier/dev:config/dev:gain-range", namespaces=self.ns)
        self.assertNotEqual(len(gain_ranges), 0)
        for gain_range in gain_ranges:
            self.assertTrue(gain_range.tag.endswith("gain-range"))
        '''
        #check amp-mode
        amp_modes = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier/dev:config/dev:amp_mode", namespaces=self.ns)
        self.assertNotEqual(len(amp_modes), 0)
        for amp_mode in amp_modes:
            self.assertTrue(amp_mode.tag.endswith("amp-mode"))
        #check target-output-power
        target_output_powers = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier/dev:config/dev:target-output-power", namespaces=self.ns)
        self.assertNotEqual(len(target_output_powers), 0)
        for target_output_power in target_output_powers:
            self.assertTrue(target_output_power.tag.endswith("target-output-power"))
        '''
        #check enabled
        enableds = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier/dev:config/dev:enabled", namespaces=self.ns)
        self.assertNotEqual(len(enableds), 0)
        for enabled in enableds:
            self.assertTrue(enabled.tag.endswith("enabled"))
        #check autolos
        autoloses = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier/dev:config/dev:autolos", namespaces=self.ns)
        self.assertNotEqual(len(autoloses), 0)
        for autolos in autoloses:
            self.assertTrue(autolos.tag.endswith("autolos"))
        #check apr-enabled
        apr_enableds = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier/dev:config/dev:apr-enabled", namespaces=self.ns)
        self.assertNotEqual(len(apr_enableds), 0)
        for apr_enabled in apr_enableds:
            self.assertTrue(apr_enabled.tag.endswith("apr-enabled"))
        #check apr-power
        apr_powers = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier/dev:config/dev:apr-power", namespaces=self.ns)
        self.assertNotEqual(len(apr_powers), 0)
        for apr_power in apr_powers:
            self.assertTrue(apr_power.tag.endswith("apr-power"))
        #check plim-enabled
        plim_enableds = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier/dev:config/dev:plim-enabled", namespaces=self.ns)
        self.assertNotEqual(len(plim_enableds), 0)
        for plim_enabled in plim_enableds:
            self.assertTrue(plim_enabled.tag.endswith("plim-enabled"))
        #check plim-power
        plim_powers = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier/dev:config/dev:plim-power", namespaces=self.ns)
        self.assertNotEqual(len(plim_powers), 0)
        for plim_power in plim_powers:
            self.assertTrue(plim_power.tag.endswith("plim-power"))

    def test_edit_edfa(self):
        filter_xml = """<nc:filter xmlns:nc="urn:ietf:params:xml:ns:netconf:base:1.0">
            <open-optical-device xmlns="http://org/openroadm/device">
                <optical-amplifier>
                    <amplifiers>
                       <amplifier/>
                    </amplifiers>
                </optical-amplifier>
            </open-optical-device>
            </nc:filter>"""
        reply = self.nc.get_config(source="running", filter=filter_xml)
        self.check_reply_data(reply)

        config_xml = """<nc:config xmlns:nc="urn:ietf:params:xml:ns:netconf:base:1.0">
            <open-optical-device xmlns="http://org/openroadm/device">
                <optical-amplifier xmlns:oc="http://openconfig.net/yang/optical-amplfier">
                    <amplifiers>
                        <amplifier nc:operation="{}">
                            <name>{}</name>
                            <config>
                                <name>{}</name>
                                <type>oc:EDFA</type>
                                <target-gain>{}</target-gain>
                                <target-gain-tilt>{}</target-gain-tilt>
                                <gain-range>{}</gain-range>
                                <enabled>{}</enabled>
                                <autolos>{}</autolos>
                                <apr-enabled>{}</apr-enabled>
                                <apr-power>{}</apr-power>
                                <plim-enabled>{}</plim-enabled>
                                <plim-power>{}</plim-power>
                            </config>
                        </amplifier>
                    </amplifiers>
                </optical-amplifier>
            </open-optical-device>
        </nc:config>"""

        configs = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier/dev:config", namespaces=self.ns)

        #set target-gain (expect error)
        reply = self.nc.edit_config(target='running', config=config_xml.format("merge", "ab", "ab", 36.0, 0.0, "oc:LOW_GAIN_RANGE","false", "false", "false", 99.0, "false", 12.0))
        self.check_reply_err(reply)

        #set target-gain
        reply = self.nc.edit_config(target='running', config=config_xml.format("merge", "ab", "ab", 15.0, 0.0, "oc:LOW_GAIN_RANGE","false", "false", "false", 99.0, "false", 12.0))
        self.check_reply_data(reply)
        reply = self.nc.get_config(source='running', filter=filter_xml)
        self.check_reply_data(reply)
        target_gain = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier[dev:name='ab']/dev:config/dev:target-gain", namespaces=self.ns)
        self.assertEqual(target_gain[0].text, "15.0")

        #set gain-range 
        reply = self.nc.edit_config(target='running', config=config_xml.format("merge", "ab", "ab", 15.0, 0.0, "oc:HIGH_GAIN_RANGE","false", "false", "false", 99.0, "false", 12.0))
        self.check_reply_data(reply)
        reply = self.nc.get_config(source='running', filter=filter_xml)
        self.check_reply_data(reply)
        gain_range = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier[dev:name='ab']/dev:config/dev:gain-range", namespaces=self.ns)
        self.assertEqual(gain_range[0].text, "oc-opt-amp:HIGH_GAIN_RANGE")
       
        #set target-gain-tilt (expect error)
        eply = self.nc.edit_config(target='running', config=config_xml.format("merge", "ab", "ab", 15.0, 10.0, "oc:HIGH_GAIN_RANGE","false", "false", "false", 99.0, "false", 12.0))

        #set target-gain-tilt
        reply = self.nc.edit_config(target='running', config=config_xml.format("merge", "ab", "ab", 15.0, 1.0, "oc:HIGH_GAIN_RANGE","false", "false", "false", 99.0, "false", 12.0))
        self.check_reply_data(reply)
        reply = self.nc.get_config(source='running', filter=filter_xml)
        self.check_reply_data(reply)
        target_gain_tilt = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier[dev:name='ab']/dev:config/dev:target-gain-tilt", namespaces=self.ns)
        self.assertEqual(target_gain_tilt[0].text, "1.0")

        #set enabled
        reply = self.nc.edit_config(target='running', config=config_xml.format("merge", "ab", "ab", 15.0, 1.0, "oc:HIGH_GAIN_RANGE","true", "false", "false", 99.0, "false", 12.0))
        self.check_reply_data(reply)
        reply = self.nc.get_config(source='running', filter=filter_xml)
        self.check_reply_data(reply)
        enabled = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier[dev:name='ab']/dev:config/dev:enabled", namespaces=self.ns)
        self.assertEqual(enabled[0].text, "true")

        #set autolos
        reply = self.nc.edit_config(target='running', config=config_xml.format("merge", "ab", "ab", 15.0, 1.0, "oc:HIGH_GAIN_RANGE","true", "true", "false", 99.0, "false", 12.0))
        self.check_reply_data(reply)
        reply = self.nc.get_config(source='running', filter=filter_xml)
        self.check_reply_data(reply)
        autolos = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier[dev:name='ab']/dev:config/dev:autolos", namespaces=self.ns)
        self.assertEqual(autolos[0].text, "true")

        #set apr-enabled
        reply = self.nc.edit_config(target='running', config=config_xml.format("merge", "ab", "ab", 15.0, 1.0, "oc:HIGH_GAIN_RANGE","true", "true", "true", 99.0, "false", 12.0))
        self.check_reply_data(reply)
        reply = self.nc.get_config(source='running', filter=filter_xml)
        self.check_reply_data(reply)
        apr_enabled = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier[dev:name='ab']/dev:config/dev:apr-enabled", namespaces=self.ns)
        self.assertEqual(apr_enabled[0].text, "true")

        #set apr-power
        reply = self.nc.edit_config(target='running', config=config_xml.format("merge", "ab", "ab", 15.0, 1.0, "oc:HIGH_GAIN_RANGE","true", "true", "true", 98.0, "false", 12.0))
        self.check_reply_data(reply)
        reply = self.nc.get_config(source='running', filter=filter_xml)
        self.check_reply_data(reply)
        apr_power = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier[dev:name='ab']/dev:config/dev:apr-power", namespaces=self.ns)
        self.assertEqual(apr_power[0].text, "98.0")

        #set plim-enable
        reply = self.nc.edit_config(target='running', config=config_xml.format("merge", "ab", "ab", 15.0, 1.0, "oc:HIGH_GAIN_RANGE","true", "true", "true", 98.0, "true", 12.0))
        self.check_reply_data(reply)
        reply = self.nc.get_config(source='running', filter=filter_xml)
        self.check_reply_data(reply)
        plim_enabled = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier[dev:name='ab']/dev:config/dev:plim-enabled", namespaces=self.ns)
        self.assertEqual(plim_enabled[0].text, "true")

        #set plim-power
        reply = self.nc.edit_config(target='running', config=config_xml.format("merge", "ab", "ab", 15.0, 1.0, "oc:HIGH_GAIN_RANGE","true", "true", "true", 98.0, "true", 11.0))
        self.check_reply_data(reply)
        reply = self.nc.get_config(source='running', filter=filter_xml)
        self.check_reply_data(reply)
        plim_power = reply.data.xpath("/nc:rpc-reply/nc:data/dev:open-optical-device/dev:optical-amplifier/dev:amplifiers/dev:amplifier[dev:name='ab']/dev:config/dev:plim-power", namespaces=self.ns)
        self.assertEqual(plim_power[0].text, "11.0")

        #set back
        reply = self.nc.edit_config(target='running', config=config_xml.format("merge", "ab", "ab", 12.0, 0.0, "oc:LOW_GAIN_RANGE","false", "false", "false", 99.0, "false", 12.0))
        self.check_reply_data(reply)
'''
if __name__ == '__main__':
    unittest.main()
'''

suite = unittest.TestLoader().loadTestsFromTestCase(TestEDFA)
unittest.TextTestRunner(verbosity=2).run(suite)


