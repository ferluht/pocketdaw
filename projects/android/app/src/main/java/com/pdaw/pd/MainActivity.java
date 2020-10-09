/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.pdaw.pd;

import android.Manifest;
import android.app.AlertDialog;
import android.app.NativeActivity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.DialogInterface;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.media.AudioDeviceInfo;
import android.media.AudioManager;
import android.media.midi.MidiDevice;
import android.media.midi.MidiDeviceInfo;
import android.media.midi.MidiInputPort;
import android.media.midi.MidiManager;
import android.media.midi.MidiOutputPort;
import android.media.midi.MidiReceiver;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.MarginLayoutParams;
import android.view.WindowManager.LayoutParams;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.UUID;

import static android.os.Environment.getExternalStorageDirectory;

public class MainActivity extends NativeActivity {

    private LeDeviceListAdapter mLeDeviceListAdapter;
    private BluetoothAdapter mBluetoothAdapter;
    private boolean mScanning;
    private Handler mHandler;

    private static final UUID[] MIDI_UUIDS = new UUID[] {
            UUID.fromString("03B80E5A-EDE8-4B33-A751-6CE34EC4C700")
    };

    private static final int REQUEST_ENABLE_BT = 1;
    // Stops scanning after 10 seconds.
    private static final long SCAN_PERIOD = 10000;

    private static final int PERMISSIONS_REQUEST_ACCESS_COARSE_LOCATION = 9500; // arbitrary

    // Olds permission request result
    private static final int readExternalStoragePermission = 1;
    private static final int recordAudioPermission = 2;

    static {
//        System.loadLibrary("c++_shared");
        System.loadLibrary("pocketdaw");
    }

    private HashMap<String, MidiDeviceInfo> midiDevices;
    private MidiDeviceInfo currentMidiDevice = null;

    MidiInputPort inputPort;
    MidiOutputPort outputPort;

    private native void midiEvent(byte cmd, byte val1, byte val2);

//    private native void startEngine();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //Hide toolbar
//        int SDK_INT = android.os.Build.VERSION.SDK_INT;
//        if(SDK_INT >= 19)
//        {
//            setImmersiveSticky();
//
//            View decorView = getWindow().getDecorView();
//            decorView.setOnSystemUiVisibilityChangeListener
//                    (new View.OnSystemUiVisibilityChangeListener() {
//                @Override
//                public void onSystemUiVisibilityChange(int visibility) {
//                    setImmersiveSticky();
//                }
//            });
//        }

        // Check permission
        // Since Android 23, permissions must explicitly be confirmed by user.
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED)
        {
            // We absolutly need this permission to run some of the examples, so ask it again even if it was refused before.
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, readExternalStoragePermission);
        }

        if (ContextCompat.checkSelfPermission(this,
                Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {

            ActivityCompat.requestPermissions(this,
                        new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},
                    readExternalStoragePermission);
        }

        if (ContextCompat.checkSelfPermission(this,
                Manifest.permission.RECORD_AUDIO)
                != PackageManager.PERMISSION_GRANTED) {

            ActivityCompat.requestPermissions(
                    this,
                    new String[]{Manifest.permission.RECORD_AUDIO},
                    recordAudioPermission);
        }

        midiDevices = new HashMap<String, MidiDeviceInfo>();
        mHandler = new Handler();

        getFilesDir();

        File sdcard = Environment.getExternalStorageDirectory();
        File dir = new File(sdcard.getAbsolutePath() + "/Music/pocketdaw/");
        dir.mkdirs();
        dir = new File(sdcard.getAbsolutePath() + "/.pocketdaw/");
        dir.mkdirs();


        copyAssets();
//        Files.copy(origin.toPath(), dest.toPath());

        Log.w("mem", getExternalStorageDirectory().getAbsolutePath());


        // Use this check to determine whether BLE is supported on the device.  Then you can
        // selectively disable BLE-related features.
//        if (!getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)) {
//            Toast.makeText(this, "ble_not_supported", Toast.LENGTH_SHORT).show();
//            finish();
//        }

        // Initializes a Bluetooth adapter.  For API level 18 and above, get a reference to
        // BluetoothAdapter through BluetoothManager.
//        final BluetoothManager bluetoothManager =
//                (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
//        mBluetoothAdapter = bluetoothManager.getAdapter();
//
//        // Checks if Bluetooth is supported on the device.
//        if (mBluetoothAdapter == null) {
//            Toast.makeText(this, "error_bluetooth_not_supported", Toast.LENGTH_SHORT).show();
//            finish();
//            return;
//        }

//        mLeDeviceListAdapter = new LeDeviceListAdapter();
//        mLeDeviceListAdapter.clear();
//        scanLeDevice(true);

        final MidiManager m = (MidiManager)this.getSystemService(Context.MIDI_SERVICE);

        scanMidiDevices();

        m.registerDeviceCallback(new MidiManager.DeviceCallback() {
            public void onDeviceAdded( final MidiDeviceInfo info ) {
                String name = info.getProperties().getString(MidiDeviceInfo.PROPERTY_NAME);
                midiDevices.put(name, info);
            }
            public void onDeviceRemoved( MidiDeviceInfo info ) {
                String name = info.getProperties().getString(MidiDeviceInfo.PROPERTY_NAME);
                midiDevices.remove(name);
            }
        }, null);

    }

    @Override
    protected void onStart() {
        super.onStart();
        setVolumeControlStream(AudioManager.STREAM_MUSIC);
    }

    private void copyAssets() {
        AssetManager assetManager = getAssets();
        String[] files = null;
        try {
            files = assetManager.list("");
        } catch (IOException e) {
            Log.e("tag", "Failed to get asset file list.", e);
        }
        for(String filename : files) {
            InputStream in = null;
            OutputStream out = null;
            try {
                in = assetManager.open(filename);

                String outDir = Environment.getExternalStorageDirectory().getAbsolutePath() + "/.pocketdaw/" ;

                File outFile = new File(outDir, filename);

                out = new FileOutputStream(outFile);
                copyFile(in, out);
                in.close();
                in = null;
                out.flush();
                out.close();
                out = null;
            } catch(IOException e) {
                Log.e("tag", "Failed to copy asset file: " + filename, e);
            }
        }
    }

    private void copyFile(InputStream in, OutputStream out) throws IOException {
        byte[] buffer = new byte[1024];
        int read;
        while((read = in.read(buffer)) != -1){
            out.write(buffer, 0, read);
        }
    }

    class MyReceiver extends MidiReceiver {
        public void onSend(byte[] data, int offset,
                           int count, long timestamp) throws IOException {
            midiEvent(data[offset], data[offset + 1], data[offset + 2]);
            Log.w("MIDI", String.format("received 0x%x 0x%x 0x%x", data[offset] & 0xFF, data[offset + 1] & 0xFF, data[offset + 2] & 0xFF));
            // parse MIDI or whatever
            //Toast.makeText(getApplicationContext(), "received midi", Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults)
    {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        switch (requestCode)
        {
            case readExternalStoragePermission:
            {
                if (grantResults.length > 0 && grantResults[0] != PackageManager.PERMISSION_GRANTED)
                {
                    // Permission refused, show a warning!
                    AlertDialog alertDialog = new AlertDialog.Builder(this).create();
                    alertDialog.setTitle("Warning");
                    alertDialog.setMessage("Some examples needs to load data from the sdcard, those will crash if data can't be loaded!");
                    alertDialog.setButton(AlertDialog.BUTTON_NEUTRAL, "OK", new DialogInterface.OnClickListener()
                    {
                        public void onClick(DialogInterface dialog, int which)
                        {
                            dialog.dismiss();
                        }
                    });
                    alertDialog.show();
                }
            }
        }
    }

//    @TargetApi(26)
    protected void onResume() {
        super.onResume();

        //Hide toolbar
        int SDK_INT = android.os.Build.VERSION.SDK_INT;
        if(SDK_INT >= 11 && SDK_INT < 14)
        {
            getWindow().getDecorView().setSystemUiVisibility(View.STATUS_BAR_HIDDEN);
        }
        else if(SDK_INT >= 14 && SDK_INT < 19)
        {
            getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_LOW_PROFILE);
        }
        else if(SDK_INT >= 19)
        {
            setImmersiveSticky();
        }

//        if (!mBluetoothAdapter.isEnabled()) {
//            if (!mBluetoothAdapter.isEnabled()) {
//                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
//                startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
//            }
//        }
//
//        // Initializes list view adapter.
//        mLeDeviceListAdapter = new LeDeviceListAdapter();
//        scanLeDevice(true);
    }
    // Our popup window, you will call it from your C/C++ code later

//    @TargetApi(26)
    void setImmersiveSticky() {
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
    }

    MainActivity _activity;
    PopupWindow _popupWindow;
    TextView _label;

//    @SuppressLint("InflateParams")
//    public void showUI()
//    {
//        if( _popupWindow != null )
//            return;
//
//        _activity = this;
//
//        this.runOnUiThread(new Runnable()  {
//            @Override
//            public void run()  {
//                LayoutInflater layoutInflater
//                = (LayoutInflater)getBaseContext()
//                .getSystemService(LAYOUT_INFLATER_SERVICE);
//
//                LinearLayout mainLayout = new LinearLayout(_activity);
//                MarginLayoutParams params = new MarginLayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
//                params.setMargins(0, 0, 0, 0);
//                _activity.setContentView(mainLayout, params);
//
//                // Show our UI over NativeActivity window
//                _popupWindow.showAtLocation(mainLayout, Gravity.TOP | Gravity.START, 10, 10);
//                _popupWindow.update();
//
//            }});
//    }

    protected void onPause()
    {
        super.onPause();
//        scanLeDevice(false);
//        mLeDeviceListAdapter.clear();
    }

    private void scanMidiDevices()
    {
        final MidiManager m = (MidiManager)this.getSystemService(Context.MIDI_SERVICE);
        MidiDeviceInfo[] devices = m.getDevices();

        for (MidiDeviceInfo device : devices) {
            String name = device.getProperties().getString(MidiDeviceInfo.PROPERTY_NAME);
            midiDevices.put(name, device);
        }
    }

    public String[] getMidiDevices()
    {
        String[] ret = new String[midiDevices.size()];

        int i = 0;
        for (String key : midiDevices.keySet()) {
            ret[i]= key;
            i++;
        }

        return ret;
    }

    public String getStorageRoot()
    {
        return getExternalStorageDirectory().getAbsolutePath();
    }

    public String[] getInputAudioDevices()
    {
        AudioManager audioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);

        AudioDeviceInfo[] info = audioManager.getDevices(AudioManager.GET_DEVICES_INPUTS);

        String[] ret = new String[info.length];

        for (int i = 0; i < info.length; i++) {
            ret[i] = String.format("input %d: %s, %s", i, info[i].getProductName(), info[i].getId());
        }

        return ret;
    }

    public String[] getOutputAudioDevices()
    {
        AudioManager audioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);

        AudioDeviceInfo[] info = audioManager.getDevices(AudioManager.GET_DEVICES_OUTPUTS);

        String[] ret = new String[info.length];

        for (int i = 0; i < info.length; i++) {
            ret[i] = String.format("output %d: %s, %s", i, info[i].getProductName(), info[i].getId());
        }

        return ret;
    }

    public void connectMidiDevice(String name)
    {
        final MidiManager m = (MidiManager)this.getSystemService(Context.MIDI_SERVICE);
        MidiDeviceInfo info = midiDevices.get(name);
        // close midi device
        if (info != null) {
            m.openDevice(info, new MidiManager.OnDeviceOpenedListener() {
                @Override
                public void onDeviceOpened(MidiDevice device) {
                    if (device == null) {
                        Log.e("", "could not open device");
                    } else {
                        outputPort = device.openOutputPort(0);
                        outputPort.connect(new MainActivity.MyReceiver());

                        inputPort = device.openInputPort(0);
                    }
                }
            }, null);
        }
    }

    public void sendMidi(byte [] msg, int offset, int length, long timestamp)
    {
        if (inputPort != null) {
            try {
                inputPort.send(msg, offset, length);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
//        else {
//            Toast.makeText(getApplicationContext(), "input null", Toast.LENGTH_SHORT).show();
//        }
    }

    private void scanLeDevice(final boolean enable) {
        if (enable) {
            startScanningIfPermitted();
        } else {
            mScanning = false;
            mBluetoothAdapter.stopLeScan(mLeScanCallback);
        }
        invalidateOptionsMenu();
    }

    private void startScanningIfPermitted() {
        if (checkSelfPermission(Manifest.permission.ACCESS_COARSE_LOCATION)
                != PackageManager.PERMISSION_GRANTED) {
            if (shouldShowRequestPermissionRationale(Manifest.permission.ACCESS_COARSE_LOCATION)) {
                Toast.makeText(this, "why_btle_location", Toast.LENGTH_LONG).show();
            }
            requestPermissions(new String[]{Manifest.permission.ACCESS_COARSE_LOCATION},
                    PERMISSIONS_REQUEST_ACCESS_COARSE_LOCATION);
        } else {
            startScanningLeDevices();
        }
    }

    private void startScanningLeDevices() {
        // Stops scanning after a pre-defined scan period.
        mHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                mScanning = false;
                mBluetoothAdapter.stopLeScan(mLeScanCallback);
                invalidateOptionsMenu();
            }
        }, SCAN_PERIOD);

        mScanning = true;
        mBluetoothAdapter.startLeScan(MIDI_UUIDS, mLeScanCallback);
    }

    // Adapter for holding devices found through scanning.
    private class LeDeviceListAdapter extends BaseAdapter {
        private ArrayList<BluetoothDevice> mLeDevices;

        public LeDeviceListAdapter() {
            super();
            mLeDevices = new ArrayList<BluetoothDevice>();
        }

        public void addDevice(BluetoothDevice device) {
            if(!mLeDevices.contains(device)) {
                mLeDevices.add(device);
            }
        }

        public BluetoothDevice getDevice(int position) {
            return mLeDevices.get(position);
        }

        public void clear() {
            mLeDevices.clear();
        }

        @Override
        public int getCount() {
            return mLeDevices.size();
        }

        @Override
        public Object getItem(int i) {
            return mLeDevices.get(i);
        }

        @Override
        public long getItemId(int i) {
            return i;
        }

        @Override
        public View getView(int i, View view, ViewGroup viewGroup) {
            ViewHolder viewHolder;
            // General ListView optimization code.
            if (view == null) {
                viewHolder = new ViewHolder();
                view.setTag(viewHolder);
            } else {
                viewHolder = (ViewHolder) view.getTag();
            }

            BluetoothDevice device = mLeDevices.get(i);
            final String deviceName = device.getName();
            if (deviceName != null && deviceName.length() > 0)
                viewHolder.deviceName.setText(deviceName);
            else
                viewHolder.deviceName.setText("unknown device");
            viewHolder.deviceAddress.setText(device.getAddress());

            return view;
        }
    }

    // Device scan callback.
    private BluetoothAdapter.LeScanCallback mLeScanCallback =
            new BluetoothAdapter.LeScanCallback() {

                @Override
                public void onLeScan(final BluetoothDevice device, int rssi, byte[] scanRecord) {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            mLeDeviceListAdapter.addDevice(device);
                            mLeDeviceListAdapter.notifyDataSetChanged();
                        }
                    });
                }
            };

    static class ViewHolder {
        TextView deviceName;
        TextView deviceAddress;
    }

}


