/****************************************************************************
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2011      Zynga Inc.
 Copyright (c) 2013-2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org
 ****************************************************************************/
/*
Copyright (c) 2014 Mudafar
GPLv3
*/

package org.cocos2dx.cpp;

import java.lang.System;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.annotation.TargetApi;
import android.content.Context;
import android.graphics.Color;
import android.graphics.Point;
import android.os.Build;
import android.os.Bundle;
import android.telephony.TelephonyManager;
import android.view.Display;
import android.view.View;
import android.view.WindowManager;
import android.widget.LinearLayout;

import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.InterstitialAd;

public class AppActivity extends Cocos2dxActivity {

    private static AppActivity _appActiviy;
    private InterstitialAd interstitialAd;
    private AdView adView;
    private static final String AD_BANNER_UNIT_ID = "ca-app-pub-2114570930052267/9054974634";
    private static final String AD_UNIT_ID = "ca-app-pub-2114570930052267/4661801032";

    // Helper get display screen to avoid deprecated function use
    private Point getDisplaySize(Display d)
    {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB)
        {
            return getDisplaySizeGE11(d);
        }
        return getDisplaySizeLT11(d);
    }

    @TargetApi(Build.VERSION_CODES.HONEYCOMB_MR2)
    private Point getDisplaySizeGE11(Display d)
    {
        Point p = new Point(0, 0);
        d.getSize(p);
        return p;
    }
    private Point getDisplaySizeLT11(Display d)
    {
        try
        {
            Method getWidth = Display.class.getMethod("getWidth", new Class[] {});
            Method getHeight = Display.class.getMethod("getHeight", new Class[] {});
            return new Point(((Integer) getWidth.invoke(d, (Object[]) null)).intValue(), ((Integer) getHeight.invoke(d, (Object[]) null)).intValue());
        }
        catch (NoSuchMethodException e2) // None of these exceptions should ever occur.
        {
            return new Point(-1, -1);
        }
        catch (IllegalArgumentException e2)
        {
            return new Point(-2, -2);
        }
        catch (IllegalAccessException e2)
        {
            return new Point(-3, -3);
        }
        catch (InvocationTargetException e2)
        {
            return new Point(-4, -4);
        }
    }




    @Override
    protected void onCreate(Bundle savedInstanceState){
        System.out.println("FUNKY BULL SHRIMP!!");
        super.onCreate(savedInstanceState);

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        int width = getDisplaySize(getWindowManager().getDefaultDisplay()).x;
        int height = getDisplaySize(getWindowManager().getDefaultDisplay()).y;

        LinearLayout.LayoutParams adParams = new LinearLayout.LayoutParams(
                width,
                LinearLayout.LayoutParams.MATCH_PARENT);
        interstitialAd = new InterstitialAd(this);

        adView = new AdView(this);

        adView.setAdSize(AdSize.SMART_BANNER);
      //  interstitialAd.setAdSize(new AdSize(width, height));
        //interstitialAd.setAdSize(new AdSize(width, height));
        adView.setY(height / 2 - AdSize.BANNER.getHeight() / 2);
        //adView.setY(0);
        adView.setId(999);
        adView.setAdUnitId(AD_BANNER_UNIT_ID);

        interstitialAd.setAdUnitId(AD_UNIT_ID);

        final TelephonyManager tm =(TelephonyManager)getBaseContext().getSystemService(Context.TELEPHONY_SERVICE);

        String deviceid = tm.getDeviceId();
        AdRequest adRequest = new AdRequest.Builder()
                .addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
                .addTestDevice("5FF56AEF6D51128B97409C80D2EF811F")
                .addTestDevice("1A120123D7D8AD6EC6A15AB9DCACF05C")
                .addTestDevice("94BB4BECE4ED30F299A4C8861E1B3F6C")
                .build();

        interstitialAd.loadAd(adRequest);
        adView.loadAd(adRequest);

        //adView.setBackgroundColor(Color.BLACK);
        adView.setBackgroundColor(0);
        //adView.setBackgroundColor(Color.rgb(152, 127, 82));
        addContentView(adView, adParams);

        _appActiviy = this;

    }


    public static void hideBanner()
    {
        System.out.println("HIDE AD INITIATED");
        _appActiviy.runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                if (_appActiviy.adView.isEnabled())
                    _appActiviy.adView.setEnabled(false);
                if (_appActiviy.adView.getVisibility() != View.INVISIBLE)
                    _appActiviy.adView.setVisibility(View.INVISIBLE);
            }
        });
    }


    public static void showAd()
    {
        _appActiviy.runOnUiThread(new Runnable()
        {

            @Override
            public void run()
            {
                _appActiviy.interstitialAd.show();
            }
        });

    }

    public static void showBanner(){
        System.out.println("SHOW BANNER INITIATED");
        _appActiviy.runOnUiThread(new Runnable()
        {

            @Override
            public void run()
            {
                if (!_appActiviy.adView.isEnabled()) {
                    _appActiviy.adView.setEnabled(true);
                }
                if (_appActiviy.adView.getVisibility() == View.INVISIBLE) {
                System.out.println("BANNER SHOULD BE VISIBLE NOW");
                    _appActiviy.adView.setVisibility(View.VISIBLE);
                }
            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (adView != null) {
            adView.resume();
        }
    }

    @Override
    protected void onPause() {
        if (adView != null) {
            adView.pause();
        }
        super.onPause();
    }

    @Override
    protected void onDestroy() {
        adView.destroy();
        super.onDestroy();
    }

}