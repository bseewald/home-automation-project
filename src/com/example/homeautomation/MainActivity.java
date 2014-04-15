package com.example.homeautomation;

import com.example.homeautomation.R;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

/**
 * @author Bruna Seewald <brubetinha@gmail.com> 
 *
 */

public class MainActivity extends Activity implements OnClickListener {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		final Button startBtn = (Button) this.findViewById(R.id.ConnectApp);
		startBtn.setOnClickListener(this);

	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch(v.getId()) {
		
			case R.id.ConnectApp: {
				
				//Verificação se o wifi está ON
			    
				
				//Estando ligado segue para a tela dos sensores (sockets aqui ?)
				final Intent intent = new Intent(this, AcquirementActivity.class);
				this.startActivity(intent);
			}	
		}	
	}
	
}
