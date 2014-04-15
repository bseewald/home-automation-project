/**
 * 
 */
package com.example.homeautomation;

import android.app.Activity;
import android.os.Bundle;
import android.widget.ImageView;

/**
 * @author Bruna Seewald <brubetinha@gmail.com> 
 *
 */
public class AcquirementActivity extends Activity {

	private ImageView temp, motion, propane, light;
	/**
	 * @param args
	 */
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {

		super.onCreate(savedInstanceState);		
		setContentView(R.layout.acquirement_layout);	
		
	}	
}
