package com.wifintercom.wifintercom;

import android.content.Context;
import android.widget.Toast;

import com.android.volley.DefaultRetryPolicy;
import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.RetryPolicy;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import java.io.Serializable;
import java.net.InetAddress;
import java.util.HashMap;
import java.util.Map;

public class WiFintercom implements Serializable {
    protected InetAddress ip;
    protected String hostname;
    protected String urlBase;
    protected String urlOpenTheGate;

    WiFintercom(InetAddress ip, String hostname){
        this.ip = ip;
        this.hostname = hostname;

        this.urlBase = "http://" + ip.getHostAddress();
        this.urlOpenTheGate = this.urlBase + "/openthegatebitch";
    }

    public String GetHostname(){return hostname;}

    public void OpenTheGate(final String passwd, final Context context){
        StringRequest postRequest = new StringRequest(Request.Method.POST, urlOpenTheGate,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
                        String parts[] = response.split("<br>");
                        Toast toast = Toast.makeText(context, parts[0], Toast.LENGTH_SHORT);
                        toast.show();
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        error.printStackTrace();
                    }
                }
        ) {
            @Override
            protected Map<String, String> getParams()
            {
                Map<String, String>  params = new HashMap<>();
                // the POST parameters:
                params.put("passwd", passwd);
                return params;
            }
        };
        RetryPolicy mRetryPolicy = new DefaultRetryPolicy(
                0,
                DefaultRetryPolicy.DEFAULT_MAX_RETRIES,
                DefaultRetryPolicy.DEFAULT_BACKOFF_MULT);

        postRequest.setRetryPolicy(mRetryPolicy);
        Volley.newRequestQueue(context).add(postRequest);
    }

}
