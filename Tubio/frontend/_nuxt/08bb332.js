(window.webpackJsonp=window.webpackJsonp||[]).push([[3],{252:function(t,e,o){var content=o(258);"string"==typeof content&&(content=[[t.i,content,""]]),content.locals&&(t.exports=content.locals);(0,o(16).default)("ac8f1938",content,!0,{sourceMap:!1})},253:function(t,e,o){"use strict";o.r(e);var n={props:{height:{type:String,default:"0"},m_height:{type:String,default:"0"}},computed:{mobile_height:function(){return"0"===this.m_height?this.height:this.m_height}}},l=(o(257),o(8)),component=Object(l.a)(n,(function(){var t=this.$createElement;return(this._self._c||t)("div",{staticClass:"spacer",style:"--height: "+this.height+"; --m_height: "+this.mobile_height+";"})}),[],!1,null,"70a5daf0",null);e.default=component.exports},254:function(t,e,o){var content=o(271);"string"==typeof content&&(content=[[t.i,content,""]]),content.locals&&(t.exports=content.locals);(0,o(16).default)("1d10f8d7",content,!0,{sourceMap:!1})},255:function(t,e,o){var content=o(273);"string"==typeof content&&(content=[[t.i,content,""]]),content.locals&&(t.exports=content.locals);(0,o(16).default)("3edd6b06",content,!0,{sourceMap:!1})},256:function(t,e,o){var content=o(275);"string"==typeof content&&(content=[[t.i,content,""]]),content.locals&&(t.exports=content.locals);(0,o(16).default)("4667df8d",content,!0,{sourceMap:!1})},257:function(t,e,o){"use strict";var n=o(252);o.n(n).a},258:function(t,e,o){(e=o(15)(!1)).push([t.i,".spacer[data-v-70a5daf0]{width:1px;height:var(--height)}@media(max-width:660px){.spacer[data-v-70a5daf0]{height:var(--m_height)}}",""]),t.exports=e},259:function(t,e,o){var content=o(277);"string"==typeof content&&(content=[[t.i,content,""]]),content.locals&&(t.exports=content.locals);(0,o(16).default)("0a79a073",content,!0,{sourceMap:!1})},265:function(t,e,o){"use strict";o.r(e);var n=o(8),l=Object(n.a)({},(function(){var t=this.$createElement,e=this._self._c||t;return e("svg",{staticClass:"bi bi-toggle-on",attrs:{viewBox:"-4 -4 24 24",fill:"#8a54a2",xmlns:"http://www.w3.org/2000/svg"}},[e("path",{attrs:{"fill-rule":"evenodd",d:"M5 3a5 5 0 0 0 0 10h6a5 5 0 0 0 0-10H5zm6 9a4 4 0 1 0 0-8 4 4 0 0 0 0 8z"}})])}),[],!1,null,null,null).exports,r=Object(n.a)({},(function(){var t=this.$createElement,e=this._self._c||t;return e("svg",{staticClass:"bi bi-toggle-off",attrs:{viewBox:"-4 -4 24 24",xmlns:"http://www.w3.org/2000/svg"}},[e("path",{attrs:{"fill-rule":"evenodd",d:"M11 4a4 4 0 0 1 0 8H8a4.992 4.992 0 0 0 2-4 4.992 4.992 0 0 0-2-4h3zm-6 8a4 4 0 1 1 0-8 4 4 0 0 1 0 8zM0 8a5 5 0 0 0 5 5h6a5 5 0 0 0 0-10H5a5 5 0 0 0-5 5z"}})])}),[],!1,null,null,null).exports,c={props:{isOn:{type:Boolean,default:!1}},components:{IconToggleOn:l,IconToggleOff:r}},d=(o(270),Object(n.a)(c,(function(){var t=this.$createElement,e=this._self._c||t;return e("div",[e("div",{staticClass:"toggle-button"},[this.isOn?e("IconToggleOn"):e("IconToggleOff")],1)])}),[],!1,null,"7e3e3a36",null));e.default=d.exports},266:function(t,e,o){"use strict";o.r(e);var n={props:{mode:{type:String,default:"normal"},message:{type:String}}},l=(o(272),o(8)),component=Object(l.a)(n,(function(){var t=this.$createElement,e=this._self._c||t;return e("div",[e("p",{class:this.mode},[this._v("\n        "+this._s(this.message)+"\n    ")])])}),[],!1,null,"6828301b",null);e.default=component.exports},267:function(t,e,o){"use strict";o.r(e);o(49);var n={components:{LogEntry:o(266).default},computed:{logs:function(){return this.$store.state.logs.logs}},mounted:function(){var t=this;this.$store.dispatch("logs/update"),setInterval((function(){t.$store.dispatch("logs/update")}),1e3)}},l=(o(274),o(8)),component=Object(l.a)(n,(function(){var t=this.$createElement,e=this._self._c||t;return e("div",{staticClass:"log-box"},this._l(this.logs,(function(t,o){return e("LogEntry",{key:o,attrs:{mode:0===t.type?"normal":1===t.type?"warn":"error",message:t.compiledMessage}})})),1)}),[],!1,null,"fc279734",null);e.default=component.exports;installComponents(component,{LogEntry:o(266).default})},270:function(t,e,o){"use strict";var n=o(254);o.n(n).a},271:function(t,e,o){(e=o(15)(!1)).push([t.i,".toggle-button[data-v-7e3e3a36]{width:50px;overflow:visible;stroke-width:1px;fill:#bbb;stroke:#bbb;cursor:pointer;transition:fill .1s,stroke .1s}.toggle-button[data-v-7e3e3a36]:hover{fill:#fff;stroke:#fff}",""]),t.exports=e},272:function(t,e,o){"use strict";var n=o(255);o.n(n).a},273:function(t,e,o){(e=o(15)(!1)).push([t.i,".warn[data-v-6828301b]{background-color:rgba(255,204,0,.26667)}.error[data-v-6828301b],.warn[data-v-6828301b]{color:#bbb;font-size:14pt}.error[data-v-6828301b]{background-color:rgba(221,68,0,.26667)}.normal[data-v-6828301b]{color:#bbb;font-size:14pt}",""]),t.exports=e},274:function(t,e,o){"use strict";var n=o(256);o.n(n).a},275:function(t,e,o){(e=o(15)(!1)).push([t.i,".log-box[data-v-fc279734]{width:100%;height:600px;border-radius:5px;padding:20px;background-color:hsla(0,0%,100%,.33333);overflow-y:scroll}",""]),t.exports=e},276:function(t,e,o){"use strict";var n=o(259);o.n(n).a},277:function(t,e,o){(e=o(15)(!1)).push([t.i,"h1[data-v-1ffe8ccc]{font-size:32pt}h1[data-v-1ffe8ccc],h2[data-v-1ffe8ccc]{color:#bbb}h2[data-v-1ffe8ccc]{font-size:24pt}.disk-usage__entry p[data-v-1ffe8ccc],.versions__entry p[data-v-1ffe8ccc]{color:#bbb;font-size:16pt}.disk-usage__entry p[data-v-1ffe8ccc]{width:150px}textarea[data-v-1ffe8ccc]{color:#04254e;border-radius:5px;font-size:16pt;padding:.7em;min-height:200px;transition:background-color .2s}textarea.inputfield--error[data-v-1ffe8ccc]{background-color:#f77}.option[data-v-1ffe8ccc]{color:#bbb;font-size:18pt;height:50px}.option.toggle>p[data-v-1ffe8ccc]{width:300px}.option.text>input[data-v-1ffe8ccc]{width:240px;text-align:right;border-radius:5px;font-size:16pt;height:30px;padding-left:.7em;padding-right:.7em}@media(max-width:768px){.option.text>input[data-v-1ffe8ccc]{width:180px}}.option.text.narrow>input[data-v-1ffe8ccc]{width:120px}@media(min-width:1024px){.border-right[data-v-1ffe8ccc]{border-right:1px solid #888}.border-left[data-v-1ffe8ccc]{border-left:1px solid #888}}.button[data-v-1ffe8ccc]{color:#000;font-family:ZillaSlab,serif;font-size:18pt;width:200px;transition:background-color .2s,opacity .2s}@media(max-width:768px){.button[data-v-1ffe8ccc]{width:150px;font-size:12pt}}.button[data-v-1ffe8ccc]:hover{background-color:#d40}.button--small[data-v-1ffe8ccc]{font-size:14pt;width:150px;height:30px}.button--nowarn[data-v-1ffe8ccc]:hover{background-color:#5954a4}.button--disabled[data-v-1ffe8ccc]{cursor:pointer;pointer-events:none;opacity:.5}hr[data-v-1ffe8ccc]{border:none;border-bottom:2px solid #888}@-webkit-keyframes goback-floating-data-v-1ffe8ccc{50%{left:1.2em}0%{left:1em}}@keyframes goback-floating-data-v-1ffe8ccc{50%{left:1.2em}0%{left:1em}}.go-back[data-v-1ffe8ccc]{position:absolute;left:1em;top:1em;font-size:34pt;font-family:ZillaSlab,serif;color:#bee7f4;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none;cursor:pointer;-webkit-animation:goback-floating-data-v-1ffe8ccc 1s infinite;animation:goback-floating-data-v-1ffe8ccc 1s infinite}",""]),t.exports=e},287:function(t,e,o){"use strict";o.r(e);o(49);var n=o(6),l=o.n(n),r=o(265),c=o(267),d=o(253),f={components:{Toggle:r.default,LogBox:c.default,Spacer:d.default},data:function(){return{canUpdate:{type:Boolean,default:!0},version__webUI:.75}},computed:{diskUsage:function(){return this.$store.state.diskUsage.usage},serverOs:function(){return this.$store.state.serverOs.os_name},serverVersion:function(){return this.$store.state.serverVersion.serverVersion}},methods:{clearDLCache:function(){var t=this;l.a.post("/api",{request:"clear_download_cache"}).then((function(e){"OK"===e.data.status&&t.$store.dispatch("dlcache/update",t)}))},clearLogs:function(){var t=this;l.a.post("/api",{request:"clear_logs"}).then((function(e){"OK"===e.data.status&&t.$store.dispatch("logs/update",t)}))},updateYtdl:function(){var t=this;l.a.post("/api",{request:"update_dep_youtubedl"}).then((function(e){"OK"===e.data.status&&t.$store.dispatch("logs/update",t)}))},killServer:function(){l.a.post("/api",{request:"kill_yourself"}).then((function(t){"OK"===t.data.status&&window.close()}))},resetToDefaults:function(){this.$store.dispatch("settings/resetToDefaults")},getSettings:function(){return this.$store.state.settings.config},setSettings_toggle_showConsole:function(){var t=this.getSettings();t.general.show_console=!t.general.show_console,this.$store.commit("settings/updateGet",t),this.$store.dispatch("settings/updateSet",t)},setSettings_toggle_onlyAllowLocalhost:function(){var t=this.getSettings();t.access.only_allow_localhost=!t.access.only_allow_localhost,this.$store.commit("settings/updateGet",t),this.$store.dispatch("settings/updateSet",t)},setSettings_toggle_enableWhitelist:function(){var t=this.getSettings();t.access.enable_whitelist=!t.access.enable_whitelist,this.$store.commit("settings/updateGet",t),this.$store.dispatch("settings/updateSet",t),t.access.enable_whitelist&&this.whitelistToInputfield()},setSettings_pushCurrentStore:function(){this.$store.dispatch("settings/updateSet",this.getSettings()),this.canUpdate=!0},whitelistToInputfield:function(){if(void 0!==this.$refs.whitelist_input){var t=JSON.stringify(this.getSettings().access.whitelist,void 0,2);return this.$refs.whitelist_input.value=t,t}setTimeout(this.whitelistToInputfield,200)},saveWhitelist:function(){var t=this.$refs.whitelist_input.value,element=this.$refs.whitelist_input;try{var e=JSON.parse(t),o=this.getSettings();o.access.whitelist=e,this.$store.commit("settings/updateGet",o),this.whitelistToInputfield(),this.$store.dispatch("settings/updateSet",o),element.classList.remove("inputfield--error")}catch(e){this.$refs.whitelist_input.value=t,element.classList.add("inputfield--error"),this.$refs.button_saveWhitelist.classList.add("button--disabled")}},updateWhitelist:function(){var t=this;this.$store.dispatch("settings/updateGet",(function(){t.whitelistToInputfield(),t.whitelistInputJsonSyntaxCheck()}))},whitelistInputJsonSyntaxCheck:function(){var element=this.$refs.whitelist_input;try{JSON.parse(element.value),element.classList.remove("inputfield--error"),this.$refs.button_saveWhitelist.classList.remove("button--disabled")}catch(t){element.classList.add("inputfield--error"),this.$refs.button_saveWhitelist.classList.add("button--disabled")}}},mounted:function(){var t=this;this.$store.dispatch("diskUsage/update"),this.$store.dispatch("serverOs/update"),this.$store.dispatch("serverVersion/update"),this.$store.dispatch("settings/updateGet",(function(){t.whitelistToInputfield()})),setInterval((function(){t.canUpdate&&t.$store.dispatch("settings/updateGet")}),1e3)}},h=(o(276),o(8)),component=Object(h.a)(f,(function(){var t=this,e=t.$createElement,o=t._self._c||e;return o("div",[o("h1",[t._v("Settings")]),t._v(" "),o("Spacer",{attrs:{height:"2em"}}),t._v(" "),o("div",{staticClass:"flex-col w-full"},[o("div",{staticClass:"flex flex-col lg:flex-row justify-content-start w-full"},[o("div",{staticClass:"w-full lg:w-1/2 border-right lg:pr-3"},[void 0!==t.getSettings().general?o("div",["Windows"===t.serverOs?o("div",{staticClass:"option toggle flex justify-between items-center"},[o("p",[t._v("Show console")]),t._v(" "),o("div",{on:{click:t.setSettings_toggle_showConsole}},[o("Toggle",{attrs:{isOn:t.getSettings().general.show_console}})],1)]):t._e(),t._v(" "),o("div",{staticClass:"option text narrow flex justify-between w-full items-center"},[o("p",{staticClass:"mr-3"},[t._v("Max speed")]),t._v(" "),o("input",{directives:[{name:"model",rawName:"v-model",value:t.getSettings().downloader.max_dlrate_per_thread,expression:"getSettings().downloader.max_dlrate_per_thread"}],attrs:{type:"text",id:"max_speed",name:"max_speed",placeholder:"100M"},domProps:{value:t.getSettings().downloader.max_dlrate_per_thread},on:{focusout:t.setSettings_pushCurrentStore,focus:function(e){t.canUpdate=!1},input:function(e){e.target.composing||t.$set(t.getSettings().downloader,"max_dlrate_per_thread",e.target.value)}}})]),t._v(" "),o("div",{staticClass:"option text narrow flex justify-between w-full items-center"},[o("p",{staticClass:"mr-3"},[t._v("Download threads")]),t._v(" "),o("input",{directives:[{name:"model",rawName:"v-model.number",value:t.getSettings().downloader.num_threads,expression:"getSettings().downloader.num_threads",modifiers:{number:!0}}],attrs:{type:"number",id:"max_threads",name:"max_threads",placeholder:"10"},domProps:{value:t.getSettings().downloader.num_threads},on:{focusout:t.setSettings_pushCurrentStore,focus:function(e){t.canUpdate=!1},input:function(e){e.target.composing||t.$set(t.getSettings().downloader,"num_threads",t._n(e.target.value))},blur:function(e){return t.$forceUpdate()}}})]),t._v(" "),o("div",{staticClass:"option toggle flex justify-between items-center"},[o("p",[t._v("Only allow localhost")]),t._v(" "),o("div",{on:{click:t.setSettings_toggle_onlyAllowLocalhost}},[o("Toggle",{attrs:{isOn:t.getSettings().access.only_allow_localhost}})],1)]),t._v(" "),o("div",{staticClass:"option toggle flex justify-between items-center"},[o("p",[t._v("Enable whitelist")]),t._v(" "),o("div",{on:{click:t.setSettings_toggle_enableWhitelist}},[o("Toggle",{attrs:{isOn:t.getSettings().access.enable_whitelist}})],1)]),t._v(" "),o("Spacer",{attrs:{height:"2em"}}),t._v(" "),t.getSettings().access.enable_whitelist?o("div",[o("h2",[t._v("Whitelist")]),t._v(" "),o("textarea",{ref:"whitelist_input",staticClass:"mt-2 w-full",on:{input:t.whitelistInputJsonSyntaxCheck}}),t._v(" "),o("div",{staticClass:"flex w-full"},[o("div",{ref:"button_saveWhitelist",staticClass:"button button--small button--nowarn mt-2 mr-2",on:{click:t.saveWhitelist}},[t._v("Save whitelist")]),t._v(" "),o("div",{staticClass:"button button--small button--nowarn mt-2",on:{click:t.updateWhitelist}},[t._v("Update whitelist")])])]):t._e()],1):t._e()]),t._v(" "),o("div",{staticClass:"w-full lg:w-1/2 border-left lg:pl-3 mt-6 lg:mt-0"},[o("div",{staticClass:"versions"},[o("div",{staticClass:"versions__entry"},[o("p",[t._v("Web-UI version: "+t._s(t.version__webUI))])]),t._v(" "),o("div",{staticClass:"versions__entry"},[o("p",[t._v("Server version: "+t._s(t.serverVersion))])])]),t._v(" "),o("div",{staticClass:"disk-usage mt-3"},[o("h2",[t._v("Disk usage")]),t._v(" "),o("div",{staticClass:"disk-usage__entry flex"},[o("p",{staticClass:"mr-3"},[t._v("Downloads: ")]),t._v(" "),o("p",[t._v(t._s(Math.round(t.diskUsage.dlcache/1024/1024))+" mb")])]),t._v(" "),o("div",{staticClass:"disk-usage__entry flex"},[o("p",{staticClass:"mr-3"},[t._v("Logs: ")]),t._v(" "),o("p",[t._v(t._s(Math.round(t.diskUsage.logs/1024/1024))+" mb")])]),t._v(" "),o("div",{staticClass:"disk-usage__entry flex"},[o("p",{staticClass:"mr-3"},[t._v("Dependencies: ")]),t._v(" "),o("p",[t._v(t._s(Math.round(t.diskUsage.dependencies/1024/1024))+" mb")])]),t._v(" "),o("div",{staticClass:"disk-usage__entry flex"},[o("p",{staticClass:"mr-3"},[t._v("Misc: ")]),t._v(" "),o("p",[t._v(t._s(Math.round(t.diskUsage.misc/1024/1024))+" mb")])]),t._v(" "),o("div",{staticClass:"disk-usage__entry flex"},[o("p",{staticClass:"mr-3"},[t._v("Total: ")]),t._v(" "),o("p",[t._v(t._s(Math.round(t.diskUsage.total/1024/1024))+" mb")])])]),t._v(" "),o("Spacer",{attrs:{height:"50px"}}),t._v(" "),o("div",{staticClass:"flex"},[o("div",{staticClass:"flex flex-col mr-1"},[o("div",{staticClass:"button",on:{click:t.clearDLCache}},[t._v("Clear downloads")]),t._v(" "),o("div",{staticClass:"button mt-2",on:{click:t.clearLogs}},[t._v("Clear logs")]),t._v(" "),"Windows"===t.serverOs?o("div",{staticClass:"button mt-2",on:{click:t.updateYtdl}},[t._v("Update ytdl")]):t._e()]),t._v(" "),o("div",{staticClass:"flex flex-col ml-1"},[o("div",{staticClass:"button",on:{click:t.killServer}},[t._v("Kill server")]),t._v(" "),o("div",{staticClass:"button mt-2",on:{click:t.resetToDefaults}},[t._v("Reset to defaults")])])])],1)]),t._v(" "),o("hr",{staticClass:"mt-6 lg:mt-2"}),t._v(" "),o("div",{staticClass:"pt-6"},[o("h2",{staticClass:"mb-4"},[t._v("Logs")]),t._v(" "),o("LogBox")],1)]),t._v(" "),o("div",{staticClass:"go-back hidden md:block"},[o("nuxt-link",{attrs:{exact:"",to:"/"}},[t._v("\n        << Back\n      ")])],1)],1)}),[],!1,null,"1ffe8ccc",null);e.default=component.exports;installComponents(component,{Spacer:o(253).default,Toggle:o(265).default,LogBox:o(267).default})}}]);