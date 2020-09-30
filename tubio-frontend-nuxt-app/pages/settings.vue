<template>
  <div>
      <h1>Settings</h1>
      <Spacer height="2em" />

      <div class="flex-col w-full">

        <div class="flex flex-col lg:flex-row justify-content-start w-full">

            <div class="w-full lg:w-1/2 border-right lg:pr-3">
                
                <div v-if="serverOs === 'Windows'" class="option toggle flex justify-between items-center">
                    <p>Show console</p>
                    <Toggle :isOn="false"/>
                </div>

                <div class="option toggle flex justify-between items-center">
                    <p>Limit speed</p>
                    <Toggle :isOn="false"/>
                </div>

                <div class="option text narrow flex justify-between w-full items-center">
                    <p class="mr-3">Max speed</p>
                    <input type="text" id="max_speed" name="max_speed" placeholder="100M">
                </div>

                <div class="option text narrow flex justify-between w-full items-center">
                    <p class="mr-3">Download threads</p>
                    <input type="text" id="max_threads" name="max_threads" placeholder="10">
                </div>

            </div>
            
            <div class="w-full lg:w-1/2 border-left lg:pl-3 mt-6 lg:mt-0">

                <div class="disk-usage">
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

  computed: {
    diskUsage: function() {
      return this.$store.state.diskUsage.usage;
    },
    serverOs: function() {
      return this.$store.state.serverOs.os_name;
    }
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
        request: "update_dep_youtubedl",
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
  },

  mounted() {
    this.$store.dispatch("diskUsage/update", this);
    this.$store.dispatch("serverOs/update", this);
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

.disk-usage {
  &__entry {
    & p {
      color: theme("colors.text-gray-1");
      font-size: 16pt;

      &:first-child {
        width: 150px;
      }
    }
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
  transition: background-color 0.2s;
  width: 200px;

  &:hover {
    background-color: theme("colors.text-error-1");
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
