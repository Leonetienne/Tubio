<template>
  <div>
      <h1>Settings</h1>
      <Spacer height="2em" />

      <div class="flex-col w-full">

        <div class="flex flex-col lg:flex-row justify-content-start w-full">

            <div class="w-full lg:w-1/2 border-right lg:pr-3">
                
                <div v-if="typeof(getSettings().general) != 'undefined'">
                  <div v-if="serverOs === 'Windows'" class="option toggle flex justify-between items-center">
                      <p>Show console</p>
                      <div v-on:click="setSettings_toggle_showConsole">
                        <Toggle
                          :isOn="getSettings().general.show_console"
                        />
                      </div>
                  </div>

                  <div class="option text narrow flex justify-between w-full items-center">
                      <p class="mr-3">Max speed</p>
                      <input
                        type="text"
                        id="max_speed"
                        name="max_speed"
                        placeholder="100M"
                        v-model="getSettings().downloader.max_dlrate_per_thread"
                        v-on:focusout="setSettings_pushCurrentStore"
                        v-on:focus="canUpdate = false;"
                        />
                  </div>

                  <div class="option text narrow flex justify-between w-full items-center">
                      <p class="mr-3">Download threads</p>
                      <input
                        type="number"
                        id="max_threads"
                        name="max_threads"
                        placeholder="10"
                        v-model.number="getSettings().downloader.num_threads"
                        v-on:focusout="setSettings_pushCurrentStore"
                        v-on:focus="canUpdate = false;"
                        />
                  </div>

                  <div class="option toggle flex justify-between items-center">
                    <p>Only allow localhost</p>
                    <div v-on:click="setSettings_toggle_onlyAllowLocalhost">
                      <Toggle
                        :isOn="getSettings().access.only_allow_localhost"
                      />
                    </div>
                  </div>

                  <div class="option toggle flex justify-between items-center">
                    <p>Enable whitelist</p>
                    <div v-on:click="setSettings_toggle_enableWhitelist">
                      <Toggle
                        :isOn="getSettings().access.enable_whitelist"
                      />
                    </div>
                  </div>

                <Spacer height="2em" />
                <div v-if="getSettings().access.enable_whitelist">
                  <h2>Whitelist</h2>
                  <textarea class="mt-2 w-full" ref="whitelist_input" v-on:input="whitelistInputJsonSyntaxCheck" />
                  <div class="flex w-full">
                    <div class="button button--small button--nowarn mt-2 mr-2" v-on:click="saveWhitelist" ref="button_saveWhitelist">Save whitelist</div>
                    <div class="button button--small button--nowarn mt-2" v-on:click="updateWhitelist">Update whitelist</div>
                  </div>
                </div>

              </div>

            </div>
            
            <div class="w-full lg:w-1/2 border-left lg:pl-3 mt-6 lg:mt-0">

                <div class="versions">
                  <div class="versions__entry">
                    <p>Web-UI version: {{version__webUI}}</p>
                  </div>
                  <div class="versions__entry">
                    <p>Server version: {{serverVersion}}</p>
                  </div>
                </div>

                <div class="disk-usage mt-3">
                    <h2>Disk usage</h2>

                    <div class="disk-usage__entry flex">
                        <p class="mr-3">Downloads: </p>
                        <p>{{Math.round(diskUsage.dlcache / 1024 / 1024)}} mb</p>
                    </div>
                    <div class="disk-usage__entry flex">
                        <p class="mr-3">Logs: </p>
                        <p>{{Math.round(diskUsage.logs / 1024 / 1024)}} mb</p>
                    </div>
                    <div class="disk-usage__entry flex">
                        <p class="mr-3">Dependencies: </p>
                        <p>{{Math.round(diskUsage.dependencies / 1024 / 1024)}} mb</p>
                    </div>
                    <div class="disk-usage__entry flex">
                        <p class="mr-3">Misc: </p>
                        <p>{{Math.round(diskUsage.misc / 1024 / 1024)}} mb</p>
                    </div>
                    <div class="disk-usage__entry flex">
                        <p class="mr-3">Total: </p>
                        <p>{{Math.round(diskUsage.total / 1024 / 1024)}} mb</p>
                    </div>
                </div>

                <Spacer height="50px"/>

                <div class="flex">
                  <div class="flex flex-col mr-1">
                    <div class="button" v-on:click="clearDLCache">Clear downloads</div>
                    <div class="button mt-2" v-on:click="clearLogs">Clear logs</div>
                    <div class="button mt-2" v-if="serverOs === 'Windows'" v-on:click="updateYtdl">Update ytdl</div>
                  </div>
                  <div class="flex flex-col ml-1">
                    <div class="button" v-on:click="killServer">Kill server</div>
                    <div class="button mt-2" v-on:click="resetToDefaults">Reset to defaults</div>
                  </div>
                </div>

            </div>

        </div>

        <hr class="mt-6 lg:mt-2" />

        <div class="pt-6">
            <h2 class="mb-4">Logs</h2>
            <LogBox />
        </div>

      </div>

      <div class="go-back hidden md:block">
        <nuxt-link exact to="/">
          &lt;&lt;&nbsp;Back
        </nuxt-link>
      </div>
  </div>
</template>

<script>
import axios from "axios";
import Toggle from "~/components/Toggle.vue";
import LogBox from "~/components/LogBox.vue";
import Spacer from "~/components/Spacer.vue";


export default {
  components: {
      Toggle,
      LogBox,
      Spacer
  },

  data: function() {
    return {
      canUpdate: {type: Boolean, default: true},
      version__webUI: 0.78
    };
  },

  computed: {
    diskUsage: function() {
      return this.$store.state.diskUsage.usage;
    },
    serverOs: function() {
      return this.$store.state.serverOs.os_name;
    },
    serverVersion: function() {
     return this.$store.state.serverVersion.serverVersion;
    },
  },

  methods: {
    clearDLCache: function() {
      const that = this;
      axios.post("/api", {
        request: "clear_download_cache",
      }).then(function(response){
        if (response.data.status === "OK") {
          that.$store.dispatch("dlcache/update", that);
        }
      });
     return;
    },

    clearLogs: function() {
      const that = this;
      axios.post("/api", {
        request: "clear_logs",
      }).then(function(response){
        if (response.data.status === "OK") {
          that.$store.dispatch("logs/update", that);
        }
      });
      return;
    },

    updateYtdl: function() {
      const that = this;
      axios.post("/api", {
        request: "update_dep_yt-dlp",
      }).then(function(response){
        if (response.data.status === "OK") {
          that.$store.dispatch("logs/update", that);
        }
      });
      return;
    },

    killServer: function() {
      const that = this;
      axios.post("/api", {
        request: "kill_yourself",
      }).then(function(response){
        if (response.data.status === "OK") {
          window.close();
        }
      });
      return;
    },

    resetToDefaults: function() {
      this.$store.dispatch("settings/resetToDefaults");
    },

    getSettings: function() {
      return this.$store.state.settings.config;
    },

    setSettings_toggle_showConsole: function() {
      var curConf = this.getSettings();
      curConf.general.show_console = !curConf.general.show_console;
      this.$store.commit("settings/updateGet", curConf);         // Apply changes to frontend immediately
      this.$store.dispatch("settings/updateSet", curConf); // Push to server, and update frontend again after response
      return;
    },
    setSettings_toggle_onlyAllowLocalhost: function() {
      var curConf = this.getSettings();
      curConf.access.only_allow_localhost = !curConf.access.only_allow_localhost;
      this.$store.commit("settings/updateGet", curConf);         // Apply changes to frontend immediately
      this.$store.dispatch("settings/updateSet", curConf); // Push to server, and update frontend again after response
      return;
    },
    setSettings_toggle_enableWhitelist: function() {
      var curConf = this.getSettings();
      curConf.access.enable_whitelist = !curConf.access.enable_whitelist;
      this.$store.commit("settings/updateGet", curConf);         // Apply changes to frontend immediately
      this.$store.dispatch("settings/updateSet", curConf); // Push to server, and update frontend again after response
      
      if (curConf.access.enable_whitelist) {
        this.whitelistToInputfield();
      }
      return;
    },
    setSettings_pushCurrentStore: function() {
       this.$store.dispatch("settings/updateSet", this.getSettings());
       this.canUpdate = true;
       return;
    },
    
    whitelistToInputfield: function() {
      // If this.$refs.whitelist_input is still undefined, just try again later, lmao
      if (typeof(this.$refs.whitelist_input) === "undefined") {
        setTimeout(this.whitelistToInputfield, 200);
      }
      else {
        var whitelistString = JSON.stringify(this.getSettings().access.whitelist, undefined, 2);
        this.$refs.whitelist_input.value = whitelistString;
        return whitelistString;
      }
    },

    saveWhitelist: function() {
      const prevContent = this.$refs.whitelist_input.value;
      const element = this.$refs.whitelist_input;
      try {
        const arrayWhitelist = JSON.parse(prevContent);
        var curConf = this.getSettings();
        curConf.access.whitelist = arrayWhitelist;
        this.$store.commit("settings/updateGet", curConf);         // Apply changes to frontend immediately
        this.whitelistToInputfield();
        this.$store.dispatch("settings/updateSet", curConf); // Push to server, and update frontend again after response
        element.classList.remove("inputfield--error");
      } catch (e) {
        this.$refs.whitelist_input.value = prevContent;
        element.classList.add("inputfield--error");
        this.$refs.button_saveWhitelist.classList.add("button--disabled");
      }
      return;
    },

    updateWhitelist: function() {
      const that = this;
      this.$store.dispatch("settings/updateGet", function() {
        that.whitelistToInputfield();
        that.whitelistInputJsonSyntaxCheck();
        return;
      });
      return;
    },

    whitelistInputJsonSyntaxCheck: function() {
      const element = this.$refs.whitelist_input;
      try {
        JSON.parse(element.value);
        element.classList.remove("inputfield--error");
        this.$refs.button_saveWhitelist.classList.remove("button--disabled");
      } catch (e) {
        element.classList.add("inputfield--error");
        this.$refs.button_saveWhitelist.classList.add("button--disabled");
      }
      return;
    }
  },

  mounted() {
    const that = this;

    this.$store.dispatch("diskUsage/update");
    this.$store.dispatch("serverOs/update");
    this.$store.dispatch("serverVersion/update");
    this.$store.dispatch("settings/updateGet", function() {
      that.whitelistToInputfield();
    });

    setInterval(function(){
      if (that.canUpdate) {
        that.$store.dispatch("settings/updateGet");
      }
    }, 1000);
    return;
  }
};
</script>

<style lang="scss" scoped>
h1 {
  color: theme("colors.text-gray-1");
  font-size: 32pt;
}

h2 {
  color: theme("colors.text-gray-1");
  font-size: 24pt;
}

.disk-usage,
.versions {
  &__entry {
    & p {
      color: theme("colors.text-gray-1");
      font-size: 16pt;
    }
  }
}

.disk-usage__entry p {
  width: 150px;
}

textarea {
  color: theme("colors.purple-0");
  border-radius: 5px;
  font-size: 16pt;
  padding: 0.7em;
  min-height: 200px;
  transition: background-color 0.2s;

  &.inputfield--error {
    background-color: #f77;
  }
}

.option {
  color: theme("colors.text-gray-1");
  font-size: 18pt;
  height: 50px;

  &.toggle > p {
    width: 300px;
  }

  &.text > input {
    width: 240px;
    text-align: right;
    border-radius: 5px;
    font-size: 16pt;
    height: 30px;
    padding-left: 0.7em;
    padding-right: 0.7em;

    @media (max-width: theme('screens.md')) {
      width: 180px;
    }
  }

  &.text.narrow > input {
    width: 120px;
  }
}

@media (min-width: theme('screens.lg')) {
  .border-right {
    border-right: 1px solid theme("colors.gray-1");
  }

  .border-left {
    border-left: 1px solid theme("colors.gray-1");
  }
}

.button {
  color: #000;
  font-family: ZillaSlab, serif;
  font-size: 18pt;
  width: 200px;
  transition:
    background-color 0.2s,
    opacity 0.2s;

  @media (max-width: theme('screens.md')) {
    width: 150px;
    font-size: 12pt;
  }

  &:hover {
    background-color: theme("colors.text-error-1");
  }

  &--small {
    font-size: 14pt;
    width: 150px;
    height: 30px;
  }

  &--nowarn:hover {
    background-color: theme("colors.purple-1");
  }

  &--disabled {
    cursor: pointer;
    pointer-events: none;
    opacity: 0.5;
  }
}

hr {
  border: none;
  border-bottom: 2px solid theme("colors.gray-1");
}

@keyframes goback-floating {
  0% { left: 1em; }
  50% { left: 1.2em; }
  0% { left: 1em; }
}

.go-back {
  position: absolute;
  left: 1em;
  top: 1em;
  font-size: 34pt;
  font-family: ZillaSlab, serif;
  color: theme("colors.purple-3-1");
  user-select: none;
  cursor: pointer;
  animation: goback-floating 1s infinite;
}
</style>
