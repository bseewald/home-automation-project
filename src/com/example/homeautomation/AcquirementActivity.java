/**
 * 
 */
package com.example.homeautomation;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;

import android.os.AsyncTask;
import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

/**
 * @author Bruna Seewald <brubetinha@gmail.com> 
 *
 */
public class AcquirementActivity extends Activity {
	
	private static final String TAG = "HomeAutomation";

	ToggleButton btnMove, btnGas, btnLight, btnTemp;
	Button btnUpdate, btnConnect;
	TextView tempValue;

	SharedPreferences preferences;

	//Networktask is the class to handle the socket connection
	NetworkTask networktask;

	/** Called when the activity is first created. */
	@Override
	protected void onCreate(Bundle savedInstanceState) {

		super.onCreate(savedInstanceState);		
		setContentView(R.layout.acquirement_layout);

		//connect the view and the objects
		btnMove = (ToggleButton)findViewById(R.id.btnMove);
		btnGas = (ToggleButton)findViewById(R.id.btnGas);
		btnLight = (ToggleButton)findViewById(R.id.btnLight);
		btnTemp = (ToggleButton)findViewById(R.id.btnTemp);
		btnUpdate = (Button)findViewById(R.id.btnUpdate);
		btnConnect = (Button)findViewById(R.id.btnConnect);
		tempValue = (TextView)findViewById(R.id.textValue);

		//add Event listeners
		btnMove.setOnClickListener(moveOnClickListener);
		btnGas.setOnClickListener(gasOnClickListener);
		btnLight.setOnClickListener(lightOnClickListener);
		btnTemp.setOnClickListener(tempOnClickListener);
		btnUpdate.setOnClickListener(buttonUpdateOnClickListener);
		btnConnect.setOnClickListener(buttonOnClickListener);
		
		//Create initial instance so SendDataToNetwork doesn't throw an error.
		networktask = new NetworkTask();
		
		//Restore Toggle Buttons status
		loadPreferences();
	}	

	// ----------------------- CONNECT BUTTON EVENT LISTENER - begin ----------------------------

	//Connecting Button
	private OnClickListener buttonOnClickListener = new OnClickListener() {
		public void onClick(View v){
			if(!networktask.isConnected()){
				//New instance of NetworkTask
				networktask = new NetworkTask(); 
				networktask.execute();
			}
			else{
				if(networktask!=null){
					networktask.closeSocket();
					networktask.cancel(true);
				}
			}
		}
	};

	//Toogle Buttons
	private OnClickListener moveOnClickListener = new OnClickListener() {
		public void onClick(View v){

			//Connected to the server
			if(networktask.isConnected()){
				SharedPreferences preferences = getPreferences(MODE_PRIVATE);
				SharedPreferences.Editor editor = preferences.edit();
				//Value stored
				editor.putBoolean("btnMove", btnMove.isChecked());
				editor.commit();  

				if(btnMove.isChecked()){
					//Button is OFF, turn ON and send the command to Arduino
					networktask.SendDataToNetwork("setOnMove"+'\n');
				}
				else{
					//Button is ON, turn OFF and send the command to Arduino
					networktask.SendDataToNetwork("setOffMove"+'\n');
				}
			}
			else{
				//Restored value
				SharedPreferences preferences = getPreferences(MODE_PRIVATE);
				boolean move = preferences.getBoolean("btnMove", false);
				btnMove.setChecked(move);
				Toast.makeText(AcquirementActivity.this, "Please, connect to the server.", Toast.LENGTH_LONG).show();
			}
		}
	};

	private OnClickListener gasOnClickListener = new OnClickListener() {
		public void onClick(View v){
			
			//Connected to the server
			if(networktask.isConnected()){
				SharedPreferences preferences = getPreferences(MODE_PRIVATE);
				SharedPreferences.Editor editor = preferences.edit();
				//Value stored
				editor.putBoolean("btnGas", btnGas.isChecked());
				editor.commit();

				if(btnGas.isChecked()){
					//Button is OFF, turn ON and send the command to Arduino
					networktask.SendDataToNetwork("setOnGas"+'\n');
				}
				else{
					//Button is ON, turn OFF and send the command to Arduino
					networktask.SendDataToNetwork("setOffGas"+'\n');
				}
			}
			else{
				//Restored value
				SharedPreferences preferences = getPreferences(MODE_PRIVATE);
				boolean gas = preferences.getBoolean("btnGas", false);
				btnGas.setChecked(gas);
				Toast.makeText(AcquirementActivity.this, "Please, connect to the server.", Toast.LENGTH_LONG).show();
			}
		}
	};

	private OnClickListener lightOnClickListener = new OnClickListener() {
		public void onClick(View v){
			
			//Connected to the server
			if(networktask.isConnected()){
				SharedPreferences preferences = getPreferences(MODE_PRIVATE);
				SharedPreferences.Editor editor = preferences.edit();
				//Value stored
				editor.putBoolean("btnLight", btnLight.isChecked());
				editor.commit();

				if(btnLight.isChecked()){
					//Button is OFF, turn ON and send the command to Arduino
					networktask.SendDataToNetwork("setOnLight"+'\n');
				}
				else{
					//Button is ON, turn OFF and send the command to Arduino
					networktask.SendDataToNetwork("setOffLight"+'\n');
				}
			}
			else{
				//Restored value
				SharedPreferences preferences = getPreferences(MODE_PRIVATE);
				boolean light = preferences.getBoolean("btnLight", false);
				btnLight.setChecked(light);
				Toast.makeText(AcquirementActivity.this, "Please, connect to the server.", Toast.LENGTH_LONG).show();
			}
		}
	};

	private OnClickListener tempOnClickListener = new OnClickListener() {
		public void onClick(View v){
			
			//Connected to the server
			if(networktask.isConnected()){
				SharedPreferences preferences = getPreferences(MODE_PRIVATE);
				SharedPreferences.Editor editor = preferences.edit();
				//Value stored
				editor.putBoolean("btnTemp", btnTemp.isChecked());
				editor.commit();

				if(btnTemp.isChecked()){
					//Button is OFF, turn ON and send the command to Arduino
					networktask.SendDataToNetwork("setOnTemp"+'\n');
				}
				else{
					//Button is ON, turn OFF and send the command to Arduino
					networktask.SendDataToNetwork("setOffTemp"+'\n');
				}
			}
			else{
				//Restored value
				SharedPreferences preferences = getPreferences(MODE_PRIVATE);
				boolean temp = preferences.getBoolean("btnTemp", false);
				btnTemp.setChecked(temp);
				Toast.makeText(AcquirementActivity.this, "Please, connect to the server.", Toast.LENGTH_LONG).show();
			}
		}
	};

	//Update Button
	private OnClickListener buttonUpdateOnClickListener = new OnClickListener() {
		public void onClick(View v){
			//Checks if the sensor is ON
			if(btnTemp.isChecked()){
				//Command to update de temperature value
				networktask.SendDataToNetwork("tempValue"+'\n');
				//+'\n'
			}
			else{
				Toast.makeText(AcquirementActivity.this, "Please, turn ON the temperature sensor", Toast.LENGTH_LONG).show();
			}
		}
	};

	// ----------------------- CONNECT BUTTON EVENT LISTENER - end ----------------------------   

	//  ----------------------- THE NETWORK TASK - begin ----------------------------
	public class NetworkTask extends AsyncTask<Void, byte[], Boolean>{

		//Stores the connection status
		private boolean connected = false;

		//Network Socket
		Socket nsocket; 
		//Network Input Stream
		InputStream nis; 
		//Network Output Stream
		OutputStream nos; 
		//Buffered reader to store the incoming bytes
		BufferedReader inFromServer;

		/**
		 * @return the connected
		 */
		public boolean isConnected() {
			return connected;
		}

		/**
		 * @param connected the connected to set
		 */
		public void setConnected(boolean connected) {
			this.connected = connected;
		}

		@Override
		protected void onPreExecute() {
			//change the connection status to "connected" when the task is started
			changeConnectionStatus(true);
		}

		@Override
		protected Boolean doInBackground(Void... arg0) {
			// TODO Auto-generated method stub
			boolean result = false;
			try {
				//create a new socket instance
				SocketAddress sockaddr = new InetSocketAddress("192.168.0.106", 5000); //MUDAR IP!!!
				nsocket = new Socket();
				//connect and set a 10 second connection timeout
				nsocket.connect(sockaddr,10000);
				if (nsocket.isConnected()) {
					nis = nsocket.getInputStream();
					nos = nsocket.getOutputStream();
					//"attach the inputstreamreader"
					inFromServer = new BufferedReader(new InputStreamReader(nis));
					//while connected
					while(true){
						//read the lines coming from the socket
						String msgFromServer = inFromServer.readLine();
						//store the bytes in an array
						byte[] theByteArray = msgFromServer.getBytes();
						//update the publishProgress
						publishProgress(theByteArray);
					}
				}
				//catch exceptions
			} catch (IOException e) {
				e.printStackTrace();
				result = true;
			} catch (Exception e) {
				e.printStackTrace();
				result = true;
			} finally {
				closeSocket();
			}
			return result;
		}

		//Method closes the socket
		public void closeSocket(){
			try {
				nis.close();
				nos.close();
				nsocket.close();
			} catch (IOException e) {
				e.printStackTrace();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

		//Method tries to send Strings over the socket connection
		public void SendDataToNetwork(String cmd) { 
			try {
				if (nsocket.isConnected()) {
					nos.write(cmd.getBytes());
				} else {
					Log.d(TAG,"SendDataToNetwork: Cannot send message. Socket is closed");
				}
			} catch (Exception e) {
				Log.d(TAG,"SendDataToNetwork: Message send failed. Caught an exception");
			}
		}

		//Methods is called everytime a new String is recieved from the socket connection
		@Override
		protected void onProgressUpdate(byte[]... values) {
			//Log.d(TAG,"2. ReceiveDataFromNetwork!");
			//if the recieved data is at least one byte
			if (values.length > 0) {
				//get the String from the recieved bytes
				String command=new String(values[0]);
				//if the string starts with "setTempValue"
				if(command.indexOf("setTempValue:")==0){
					//remove the command
					command=command.replace("setTempValue:", "");
					//and set the temperature value to the given value
					setTempValue(command);
				}
			}
		}

		//Method is called when task is cancelled
		@Override
		protected void onCancelled() {
			//Change the connection to "disconnected"
			changeConnectionStatus(false);
		}

		//Method is called after task execution
		@Override
		protected void onPostExecute(Boolean result) {
			if (result) {
				Log.d(TAG,"onPostExecute: Completed with an Error.");
			} else {
				Log.d(TAG,"onPostExecute: Completed.");
			}
			//Change connection staus to disconnected
			changeConnectionStatus(false);
		}

	}
	// ----------------------- THE NETWORK TASK - end ----------------------------

	// Method changes the connection status
	public void changeConnectionStatus(Boolean isConnected) {
		//Change variable
		networktask.setConnected(isConnected);
		
		//If connection established
		if(networktask.isConnected()){
			Log.d(TAG,"successfully connected to server");
			//Change Button opnnect text
			btnConnect.setText("Disconnect");
		}else{
			Log.d(TAG,"disconnected from Server!");
			//Change Button Connect text
			btnConnect.setText("Connect");
		}
	}

	//Method appends changes the value of the TextView tempValue
	public void setTempValue(String value) {
		tempValue.setText(value + "ûC");
	}

	private void loadPreferences(){
		//Restore the buttons status
		SharedPreferences preferences = getPreferences(MODE_PRIVATE);
		
		boolean move = preferences.getBoolean("btnMove", false);
		btnMove.setChecked(move);
		
		boolean gas = preferences.getBoolean("btnGas", false);
		btnGas.setChecked(gas);
		
		boolean light = preferences.getBoolean("btnLight", false);
		btnLight.setChecked(light);
		
		boolean temp = preferences.getBoolean("btnTemp", false);
		btnTemp.setChecked(temp);
	}
	
	//Method is called when the activity is about to run
	@Override
	protected void onResume(){
		super.onResume();
		loadPreferences();
	}

	//Method is called when app is closed
	@Override
	protected void onDestroy() {
		super.onDestroy();
		//In case the task is currently running
		if(networktask!=null){
			//cancel the task
			networktask.cancel(true);
		}   	 
	}
}
