package com.example.homeautomation;

import com.example.homeautomation.R;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;

/**
 * This work is licensed under a Creative Commons Attribution 4.0 International License.
 * http://creativecommons.org/licenses/by/4.0/
 * 
 * @author Bruna Seewald <brubetinha@gmail.com> 
 */

public class MainActivity extends Activity implements OnClickListener {

	/*
	 * Static instance of itself.
	 */
	public static Context context;
	
	// Services.
	private WifiManager wifiManager;
	private ConnectivityManager connManager;
	private NetworkInfo wifiState;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		// Define a static context. Useful for the anonymous events.
		context = getApplicationContext();
		
		// Initialize services instances.
		this.initializeServices();
		
		// Initialize view components.
		this.initializeComponents();
		
	}
		
	/**
	 * Initialize all services.
	 */
	private void initializeServices() {		
		// WIFI and Network service manager.
		this.wifiManager = (WifiManager) this.getSystemService(Context.WIFI_SERVICE);
		this.connManager = (ConnectivityManager) getSystemService(CONNECTIVITY_SERVICE);
	    this.wifiState = connManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
	}
	
	/**
	 * Initialize all view components.
	 */
	private void initializeComponents() {
		final Button startBtn = (Button) this.findViewById(R.id.EnterApp);
		startBtn.setOnClickListener(this);
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch(v.getId()) {
		
			case R.id.EnterApp: {
				
				//Wi-Fi state (On or Off)
				if(this.wifiManager.isWifiEnabled()){	
					//if enable, start the new activity
					if(this.wifiState.isConnected()){
						final Intent intent = new Intent(this, AcquirementActivity.class);
						this.startActivity(intent);
					}
					else{
						// Display message.
				    	Toast.makeText(MainActivity.context, "Please, WIFI is not connected.", Toast.LENGTH_LONG).show();
					}
				}
				else{
					// Display message.
			    	Toast.makeText(MainActivity.context, "Please, enable the WIFI to connect to the server.", Toast.LENGTH_LONG).show();
				}
				
			}	
		}	
	}
	
}
