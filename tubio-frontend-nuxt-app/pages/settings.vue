<template>
  <div>
      <h1>Settings</h1>
      <Spacer height="2em" />

      <div class="flex-col w-full">

        <div class="flex flex-col lg:flex-row justify-content-start w-full">

            <div class="w-full lg:w-1/2 border-right lg:pr-3">
                
                <div class="option toggle flex justify-between items-center">
                    <p>Show console</p>
                    <Toggle :isOn="false"/>
                </div>

                <div class="option toggle flex justify-between items-center">
                    <p>Use account</p>
                    <Toggle :isOn="false"/>
                </div>

                <div class="option text flex justify-between w-full items-center">
                    <p class="mr-3">Username</p>
                    <input type="text" id="username" name="username">
                </div>

                <div class="option text flex justify-between w-full items-center">
                    <p class="mr-3">Password</p>
                    <input type="password" id="password" name="password">
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
                        <p>759 mb</p>
                    </div>
                    <div class="disk-usage__entry flex">
                        <p class="mr-3">Logs: </p>
                        <p>9 mb</p>
                    </div>
                    <div class="disk-usage__entry flex">
                        <p class="mr-3">Dependencies: </p>
                        <p>312 mb</p>
                    </div>
                    <div class="disk-usage__entry flex">
                        <p class="mr-3">Misc: </p>
                        <p>3 mb</p>
                    </div>
                    <div class="disk-usage__entry flex">
                        <p class="mr-3">Total: </p>
                        <p>1567 mb</p>
                    </div>
                </div>

                <Spacer height="50px"/>

                <div class="button">Clear downloads</div>
                <div class="button mt-2">Clear logs</div>

            </div>

        </div>

        <hr class="mt-6 lg:mt-2" />

        <div class="pt-6">
            <h2 class="mb-4">Logs</h2>
            <LogBox />
        </div>

      </div>
  </div>
</template>

<script>
import Toggle from "~/components/Toggle.vue";
import LogBox from "~/components/LogBox.vue";
import Spacer from "~/components/Spacer.vue";


export default {
  components: {
      Toggle,
      LogBox,
      Spacer
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
  max-width: 200px;

  &:hover {
    background-color: theme("colors.text-error-1");
  }
}

hr {
  border: none;
  border-bottom: 2px solid theme("colors.gray-1");
}
</style>
