package com.wifintercom.wifintercom;

import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.DhcpInfo;
import android.net.wifi.WifiManager;
import android.os.StrictMode;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.InputType;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketTimeoutException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Vector;

public class DeviceSelectionActivity extends AppCompatActivity {
    protected String TAG = "DeviceSelectionActivity";
    protected int PORT_BROADCAST = 2390;
    protected int PORT_RECEIVE = 12345;
    protected int RECEIVE_TIMEOUT = 100;
    protected int DISCOVERY_PERIOD = 5000;
    Timer discoveryTimer;
    protected DatagramSocket socket;
    protected Vector<InetAddress> deviceInetAddrs = new Vector<>();
    protected List<String> deviceHostnames = new ArrayList<String>();
    protected ArrayAdapter<String> arrayAdapter;
    protected Thread listeningThread;

    private ListView lv;
    private WiFintercom intercom;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_selection);

        lv = (ListView) findViewById(R.id.listViewDevices);

        // This is the array adapter, it takes the context of the activity as a
        // first parameter, the type of list view as a second parameter and your
        // array as a third parameter.
        arrayAdapter = new ArrayAdapter<String>(
                this,
                android.R.layout.simple_list_item_1,
                deviceHostnames);
        lv.setAdapter(arrayAdapter);

        lv.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                // Get password
                SharedPreferences sharedPref = getPreferences(Context.MODE_PRIVATE);
                String passwd = sharedPref.getString(getString(R.string.passwd_id), "");

                // Send socket data
                intercom = new WiFintercom(deviceInetAddrs.elementAt(i), deviceHostnames.get(i));
                intercom.OpenTheGate(passwd, getApplicationContext());

                /*
                RGBCtrler rgbCtrler = new RGBCtrler(deviceInetAddrs.elementAt(i), deviceHostnames.get(i));
                Intent intent = new Intent(DeviceSelectionActivity.this, ColorPickingActivity.class);
                Bundle bundle = new Bundle();
                bundle.putSerializable("rgbctrler", rgbCtrler);
                intent.putExtra(EXTRA_BUNDLE, bundle);
                startActivity(intent);
                */
            }
        });

        // Create thread
        listeningThread = new Thread(new Runnable() {
            public void run() {
                try {
                    //Keep a socket open to listen to all the UDP trafic that is destined for this port
                    socket = new DatagramSocket(PORT_RECEIVE, InetAddress.getByName("0.0.0.0"));
                    socket.setBroadcast(true);

                    while (!Thread.interrupted()) {
                        Log.i(TAG, "wreceive broadcast packets!");

                        //Receive a packet
                        byte[] recvBuf = new byte[15000];
                        DatagramPacket packet = new DatagramPacket(recvBuf, recvBuf.length);
                        try {
                            socket.setSoTimeout(RECEIVE_TIMEOUT);
                            socket.receive(packet);
                            //Packet received
                            Log.i(TAG, "Packet received from: " + packet.getAddress().getHostAddress());
                            String data = new String(packet.getData()).trim();
                            Log.i(TAG, "Packet received; data: " + data);

                            //Parse data
                            final String[] strArr = data.split("-");

                            //Add device IP to storage if data is valid
                            if (strArr[0].equals("WiFintercom")) {
                                runOnUiThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        try {
                                            InetAddress addr = InetAddress.getByName(strArr[1]);    //get ipaddr
                                            // Check if it is already in the list
                                            if (deviceInetAddrs.indexOf(addr) < 0) {
                                                deviceInetAddrs.add(addr);
                                                deviceHostnames.add(strArr[2]);
                                                arrayAdapter.notifyDataSetChanged();
                                            }
                                        }catch(IOException ex) {
                                            Log.i(TAG, "Oops " + ex.getMessage());
                                        }
                                    }
                                });
                            }
                        } catch (SocketTimeoutException ex) {
                            //NOP
                        } catch (Exception ex) {
                            throw ex;
                        }
                    }
                    Log.i(TAG, "Thread interrupted");
                    if (socket != null) {
                        socket.close();
                    }
                } catch (Exception ex) {
                    Log.i(TAG, "Oops: " + ex.getMessage());
                    if (socket != null) {
                        socket.close();
                    }
                }
            }
        });

        getSupportActionBar().setTitle("Device selection");
    }

    @Override
    public void onStart() {
        clearList();
        listeningThread.start();
        discoveryTimer = new Timer();
        discoveryTimer.schedule(new DiscoveryTask(), 0, DISCOVERY_PERIOD);
        super.onStart();
    }

    @Override
    public void onStop() {
        listeningThread.interrupt();
        while(listeningThread.isAlive());
        discoveryTimer.cancel();
        discoveryTimer.purge();
        super.onStop();
    }

    public boolean onCreateOptionsMenu(Menu menu) {

        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu, menu);
        return true;
    }

    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.set_passwd:
                getPasswordDialog();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    class DiscoveryTask extends TimerTask {
        @Override
        public void run() {
            /*
            runOnUiThread(new Runnable() {
                              @Override
                              public void run() {
                                  clearList();
                              }
                          });
                          */
            sendBroadcast(String.valueOf(PORT_RECEIVE));
        }
    }

    protected void getPasswordDialog() {
        final EditText input = new EditText(this);
        input.setHint("Password");
        input.setInputType(InputType.TYPE_CLASS_TEXT |
                InputType.TYPE_TEXT_VARIATION_PASSWORD);

        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("WiFintercom password");
        builder.setMessage("Enter the password to open the gate!");
        builder.setView(input);
        builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                String passwd =  input.getText().toString();
                SharedPreferences sharedPref = getPreferences(Context.MODE_PRIVATE);
                SharedPreferences.Editor editor = sharedPref.edit();
                editor.putString(getString(R.string.passwd_id), passwd);
                editor.commit();
                dialog.dismiss();
            }
        });
        builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
            }
        });
        builder.show();
    }

    protected void sendBroadcast(String messageStr) {
        // Hack Prevent crash (sending should be done using an async task)
        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);

        try {
            //Open a random port to send the package
            DatagramSocket socket = new DatagramSocket();
            socket.setBroadcast(true);
            byte[] sendData = messageStr.getBytes();
            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, getBroadcastAddress(), PORT_BROADCAST);
            socket.send(sendPacket);
            System.out.println(getClass().getName() + "Broadcast packet sent to: " + getBroadcastAddress().getHostAddress());
        } catch (IOException e) {
            Log.e(TAG, "IOException: " + e.getMessage());
        }
    }

    InetAddress getBroadcastAddress() throws IOException {
        WifiManager wifi = (WifiManager) getApplicationContext().getApplicationContext().getSystemService(WIFI_SERVICE);
        DhcpInfo dhcp = wifi.getDhcpInfo();
        // handle null somehow

        int broadcast = (dhcp.ipAddress & dhcp.netmask) | ~dhcp.netmask;
        byte[] quads = new byte[4];
        for (int k = 0; k < 4; k++)
            quads[k] = (byte) ((broadcast >> k * 8) & 0xFF);
        return InetAddress.getByAddress(quads);
    }

    void clearList() {
        deviceHostnames.clear();
        deviceInetAddrs.clear();
        arrayAdapter.notifyDataSetChanged();
    }
}
