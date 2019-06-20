package com.pdaw.pd;

import android.app.Activity;
import android.content.Context;
import android.media.midi.MidiDevice;
import android.media.midi.MidiDeviceInfo;
import android.media.midi.MidiManager;
import android.media.midi.MidiOutputPort;
import android.media.midi.MidiReceiver;
import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;

import android.util.Log;
import android.view.MotionEvent;
import android.widget.Toast;

import java.io.IOException;

public class MainActivity extends Activity {

    static {
        System.loadLibrary("native-lib");
    }

    private native void touchEvent(int action, float x, float y);

    private native void midiReceived(byte cmd, byte val1, byte val2);

    private native void startEngine();

    private native void stopEngine();

//    private MidiManager m;


    class MyReceiver extends MidiReceiver {
        public void onSend(byte[] data, int offset,
                           int count, long timestamp) throws IOException {
            Log.e("", "could not open device " + data);
            midiReceived(data[1], data[2], data[3]);
            // parse MIDI or whatever
            //Toast.makeText(getApplicationContext(), "received midi", Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        startEngine();

        final MidiManager m = (MidiManager)this.getSystemService(Context.MIDI_SERVICE);
        MidiDeviceInfo[] infos = m.getDevices();

        //Toast.makeText(MainActivity.this, "on create", Toast.LENGTH_SHORT).show();

        m.registerDeviceCallback(new MidiManager.DeviceCallback() {
            public void onDeviceAdded( final MidiDeviceInfo info ) {
                Bundle properties = info.getProperties();
                String manufacturer = properties
                        .getString(MidiDeviceInfo.PROPERTY_MANUFACTURER);
             //   Toast.makeText(MainActivity.this, manufacturer + " connected", Toast.LENGTH_SHORT).show();

                m.openDevice(info, new MidiManager.OnDeviceOpenedListener() {
                    @Override
                    public void onDeviceOpened(MidiDevice device) {
                        if (device == null) {
                            Log.e("", "could not open device " + info);
                        } else {
                            //Toast.makeText(getApplicationContext(), "opened", Toast.LENGTH_SHORT).show();
                            MidiOutputPort outputPort = device.openOutputPort(0);
                            outputPort.connect(new MyReceiver());
                        }
                    }}, null);
            }
            public void onDeviceRemoved( MidiDeviceInfo info ) {
                Bundle properties = info.getProperties();
                String manufacturer = properties
                        .getString(MidiDeviceInfo.PROPERTY_MANUFACTURER);
                //Toast.makeText(getApplicationContext(), manufacturer + " disconnected", Toast.LENGTH_SHORT).show();
            }
        }, null);

    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        touchEvent(event.getAction(), event.getX(), event.getY());
        return super.onTouchEvent(event);
    }

    @Override
    public void onDestroy() {
        stopEngine();
        super.onDestroy();
    }
}
