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
		
/*		this.temp = (ImageView) this.findViewById(R.id.imageView1);
		this.motion = (ImageView) this.findViewById(R.id.imageView2);
		this.propane = (ImageView) this.findViewById(R.id.imageView3);
		this.light = (ImageView) this.findViewById(R.id.imageView4);
		
		temp.setImageResource(R.drawable.thermometer);
		motion.setImageResource(R.drawable.motion);
		propane.setImageResource(R.drawable.propane);
		light.setImageResource(R.drawable.lamp);*/
	}	
}
